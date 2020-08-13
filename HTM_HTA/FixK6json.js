// node FixK6json.js test1.json 1000 test1res.json xxxxxxxxxx000 xxxxxxxxxx000
//                    k6output   grp  outputData    filter from   filter to
const multiply = 1000000;
var args = process.argv.slice(2);
var fs = require('fs');
var dta = fs.readFileSync(args[0])
    .toString() // K6 \n delimitted JSON Object(s)
    .split('\n');
var last = dta.pop(); if (last) dta.push(last); // last \n !
//           min/max array |   DATA   |unused desc| min time scale   |2B unique| start/end filter args
var res = { timeInterval: 0, Point: {}, Metric: {}, groupBy: args[1] * multiply, time: args.slice(3) };
for (const r of dta) { // random sorted Object lines
    const js = JSON.parse(r);
    if (js.data.name) { // table name (of Metric ?)
        const idx = res[js.type][js.data.name];
        if (idx) idx.push(js.data);
        else res[js.type][js.data.name] = [js.data];
    } else {
        if (js.type === 'Point') {
            let key = js.metric; // table name
            /* Errors only report fatal errors, while failing requests (timeouts) are warnings... K6 issue 277
            if (js.data.tags && js.data.tags.error) { // error mentioned
                key = 'errors';
                js.data.value = 1; // change value to 1 error for chart
            } else */ if (key === 'checks') key = js.data.tags.check; // general check description
            if (!res.Point[key]) { // [time,val]| []
                res.Point[key] = { tupples: [], sortedValues: [] };
            }
            const stats = res.Point[key]; // shorcut      |          exact time value (>1e18)
            const tc = new Date(js.data.time).getTime() * multiply + parseInt(js.data.time.substr(23));
            const tcAdj = tc / multiply; // standard JS time
            const val = js.data.value;
            if (res.time.length && (tcAdj < res.time[0] || tcAdj > res.time[1])) continue; // check time interval if set
            stats.tupples.push([tc, val]);
            stats.sortedValues.push(val); // values 2B sorted for statistics
        }
    }
}
for (const i in res.Point) {
    const o = res.Point[i];
    o.tupples.sort(sort1st); // sort by time (#s 2big 2B sorted)
    const lastRow = o.tupples.length - 1;
    if (!res.timeInterval) { // set start time interval from this set
        res.timeInterval = [o.tupples[0][0], o.tupples[lastRow][0]];
    } else { // extend in case this data are any wider
        if (o.tupples[0][0] < res.timeInterval[0]) {
            res.timeInterval[0] = o.tupples[0][0];
        }
        if (o.tupples[lastRow][0] > res.timeInterval[1]) {
            res.timeInterval[1] = o.tupples[lastRow][0];
        }
    }
    o.sortedValues.sort(sortFloat); // sort values 4 stats
    const half = lastRow / 2;
    const time0 = o.tupples[0][0] - (o.tupples[0][0] % res.groupBy); // round 1st group time
    var grps = {};
    var count = 0;
    for (const r of o.tupples) { // group values by time 2 arrays
        const tg = r[0] - (r[0] % res.groupBy);
        if (!grps[tg]) grps[tg] = [r[1]];
        else grps[tg].push(r[1]);

        if (o[r[0]]) { // keep records @same time
            const step = res.groupBy / multiply;
            var pos = r[0] + step; // delay time until it is unique
            if (pos === r[0]) throw new Error('Too small step.');
            while (o[pos]) {
                pos += step;
            }
            o[pos] = r[1];
        } else o[r[0]] = r[1];
        count += r[1];
    }
    o.mean = { time: time0, value: count / o.sortedValues.length, times: [], values: [] };
    o.stats = [];
    for (const r in grps) {
        const stat = GroupStats(parseInt(r), grps[r]);
        o.stats.push(stat); // get stats of each group
        if (stat.max !== stat.min || stat.mean !== 0) {
            o.mean.times.push(stat.time);
            o.mean.values.push(stat.mean);
        }
    }
    var statNotEmpty = [];
    for (var obj of o.stats) {
        if (obj.max !== obj.min || obj.mean !== 0) {
            statNotEmpty.push(obj);
        }
    }
    o.stats = statNotEmpty;
    setTimeValueGrp(o, 'max', o.sortedValues[lastRow]);
    if (o.tupples.length & 1) { // odd => value in the middle ELSE 1/2 of pre/post value
        o.median = { time: time0, value: o.sortedValues[half] };
    } else o.median = { time: time0, value: (o.sortedValues[half - 0.5] + o.sortedValues[half + 0.5]) / 2 };
    setTimeValueGrp(o, 'min', o.sortedValues[0]);
    setTimeValueGrp(o, 'p90', o.sortedValues[parseInt(o.sortedValues.length * 0.9)]);
    setTimeValueGrp(o, 'p95', o.sortedValues[parseInt(o.sortedValues.length * 0.95)]);
    delete o.stats; // remove temps
    delete o.sortedValues;
}
fs.writeFileSync(args[2], JSON.stringify(res, null, 2));

function GroupStats (tg, grps) {
    const sortedValues = grps.concat().sort(sortFloat);
    var o = { time: tg, max: sortedValues[0], p95: null, p90: null, min: sortedValues[0] };
    var count = 0;
    for (var i of grps) {
        if (o.max < i) o.max = i;
        if (o.min > i) o.min = i;
        count += i;
    }
    o.p90 = sortedValues[parseInt(grps.length * 0.90 + 0.5) - 1];
    o.p95 = sortedValues[parseInt(grps.length * 0.95 + 0.5) - 1];
    o.mean = count / grps.length;
    return o;
}

function setTimeValue (val, tupples) {
    for (var t of tupples) {
        if (t[1] === val) {
            return {
                time: parseInt(t[0]),
                value: t[1]
            };
        }
    }
    throw new Error('Value not found');
}

function setTimeValueGrp (o, type, val) { // (src/)destObject, statName, value
    o[type] = setTimeValue(val, o.tupples); // [timeOf, val]
    var og = o[type];
    og.times = [];
    og.values = [];
    for (var r of o.stats) {
        og.times.push(r.time);
        og.values.push(r[type]);
    }
}

function sortFloat (a, b) { return a - b; }
function sort1st (a, b) { return a[0] - b[0]; }
