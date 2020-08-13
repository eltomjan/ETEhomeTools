// node FillGrafanaJSON.js test1res.json snap.json snapReplaced.json
//                       preprocessedData template  final Dashboard
const multiply = 1000000;
var args = process.argv.slice(2);
var fs = require('fs');
var dta = JSON.parse(fs.readFileSync(args[0]));
var template = JSON.parse(fs.readFileSync(args[1])); // snapped dashboard w/o data
template.time.from = new Date(dta.timeInterval[0] / multiply).toJSON(); // set longest data interval time(s)
template.time.to = new Date(dta.timeInterval[1] / multiply).toJSON();
template.uid = parseInt((dta.timeInterval[0] / multiply + dta.timeInterval[1] / multiply) / 2);
template.title += new Date(dta.timeInterval[0] / multiply).toLocaleString(); // decorate title with start time
for (var s of template.panels) {
    if (s.snapshotData === undefined) continue; // panel w/o data
    for (var sno of s.snapshotData) {
        var sql = sno.meta.executedQueryString.split(';');
        if (sql.length === 1) sql = sql[0];
        else sql = sql[s.snapshotData.indexOf(sno)]; // pick proper request
        const params = ParseSqls(sql);
        if (params[1] === 'checks') {
            params[1] = sno.name; // 'is has records', 'is has records', ...
        }
        const src = params[1]; // table name
        const groupFill = params[3];
        let tab = dta.Point[src];
        if (tab === undefined) continue;
        if (groupFill || !params[0]) { // GROUP BY or SELECT "value"
            if (params[0]) {
                tab = GetGroup(tab, params, dta.groupBy / multiply);
                setContent(sno.fields, tab);
            } else {
                tab = dta.Point[src];
                TranslateContent(sno.fields, tab, multiply);
            }
        } else { // single statistic value
            var stat = dta.Point[src][params[0]];
            if (stat) {
                stat.time = parseInt(stat.time / multiply);
                sno.fields[0].values = [stat.time];
                sno.fields[1].values = [stat.value];
                sno.fields[1].config = { max: stat.value, min: stat.value };
            }
        }
    }
}
fs.writeFileSync(args[2], JSON.stringify(template, null, 2));

function setContent (dst, src) {
    var t = dst[0]; var v;
    if (t.name !== 'Time') { // switch in case Value, Time
        v = t;
        t = dst[1];
    } else v = dst[1];
    t.values = src[0];
    v.values = src[1];
    v.config = { max: src[3], min: src[2] };
}

function TranslateContent (dst, src, multiply) {
    var t = dst[0]; var v;
    if (t.name !== 'Time') { // switch in case Value, Time
        v = t;
        t = dst[1];
    } else v = dst[1];
    v.config = { max: Number.MIN_SAFE_INTEGER, min: Number.MAX_SAFE_INTEGER };
    for (const r in src) {
        if (isNaN(r)) continue;
        const tm = parseInt(r / multiply);
        t.values.push(tm);
        v.values.push(src[r]);
        if (v.config.max < src[r]) v.config.max = src[r];
        if (v.config.min > src[r]) v.config.min = src[r];
    }
}

function ParseSqls (sql) {
    var s1 = sql.replace('\r', '')
        .replace(', "check"', '')
        .replace(/time\(1s\)/g, '')
        .replace(/ fill\(([a-z]+)\)/g, '$1')
        .replace(/SELECT /g, '')
        .replace(/ FROM /g, '\u0001')
        .replace(/ WHERE /g, '\u0002')
        .replace(/ GROUP BY /g, '\u0003')
        .replace(/[\u0001-\u0008]/g, '\u0009').split('\u0009');
    s1[0] = s1[0].replace(/[\("\)]/g, '')
        .replace('value', '')
        .replace('percentile, ', 'p');
    s1[1] = s1[1].replace(/["/^$]/g, '');
    s1[2] = s1[2].replace(/time >= \d+\S+ and time <= \d+\S+/, '')
        .replace(/^ and /, '');
    return s1;
}

function GetGroup (tab, params, step) {
    if ('|sum|count|'.indexOf(params[0]) > 0) {
        var minMax = [];
        for (const r in tab) {
            if (!isNaN(r)) {
                if (minMax.length < 2) minMax.push(parseInt(r / multiply)); // time min 1st
                else minMax[1] = parseInt(r / multiply); // time max @end
            }
        }
        var times = []; var values = [];
        var tm = minMax[0];
        tm = tm - tm % step;
        while (tm <= minMax[1]) { // generate times with step & null values
            times.push(tm);
            values.push(null);
            tm += step;
        }
        for (const r in tab) {
            if (isNaN(r)) continue;
            tm = r / multiply;
            tm -= tm % step;
            values[times.indexOf(tm)] += tab[r];
        }
        if (params[3] === 'none') {
            const filtered = [[], []];
            for (const r in values) {
                if (values[r] !== null) {
                    filtered[0].push(times[r]);
                    filtered[1].push(values[r]);
                }
            }
            times = filtered[0];
            values = filtered[1];
            if (!values.length) return [times, values].concat([Number.NaN, Number.NaN]);
            const valSrt = values.concat().sort(sortFloat);
            minMax[0] = valSrt[0];
            minMax[1] = valSrt.pop();
        } else {
            const valSrt = values.concat().sort(sortFloat);
            let i = 0;
            while (valSrt[i] === null) i++;
            minMax[0] = valSrt[i];
            minMax[1] = valSrt.pop();
        }
        return [times, values].concat(minMax);
    } else {
        const valSrt = tab[params[0]].values.concat().sort(sortFloat);
        const times = tab[params[0]].times;
        for (const t in times) {
            times[t] /= multiply;
        }
        return [
            tab[params[0]].times,
            tab[params[0]].values,
            valSrt[0],
            valSrt.pop()
        ];
    }
}

function sortFloat (a, b) { return a - b; }
