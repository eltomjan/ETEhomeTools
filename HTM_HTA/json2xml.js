// xml = json2xml(d);
function json2xml(arr) {
    var r = "";
    for(var a in arr) {
        if(arr[a].toString().indexOf("[object") == 0) {
            if(isNaN(a)) {
                r += '<' + a + '>\r\n';
            } else {
                r += '<No' + a + '>';
            }
            var sub = json2xml(arr[a]).split('\n');
            for(var b in sub) if(sub[b].length) r += sub[b];
            if(isNaN(a)) {
                r += '</' + a + '>\r\n';
            } else {
                r += '</No' + a + '>\r\n';
            }
        } else {
            r += '<' + a + ">" + arr[a] + '</' + a + '>';
        }
    }
    return r;
}
