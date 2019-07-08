// csv = json2csv(dataObj, [], [], [], []);
function json2csv(arr, head, data, out, parent) {
    var r = "";
    for(var a in arr) {
        if(arr[a].toString().indexOf("[object") == 0) {
            parent.push(a);
            var hs = parent.slice(), dts = [];
            var sub = json2csv(arr[a], hs, dts, out, parent).split('\n');
            out.push([hs]);
            out.push([parent, dts]);
            parent.pop();
            for(var b in sub) if(sub[b].length) r += a + '\t' + sub[b] + '\n';
        } else {
            r += a + "\t" + arr[a] + '\n';
            head.push(a);
            data.push(arr[a]);
        }
    }
    return r;
}
