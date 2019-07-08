/*
var csv = []; // CSV result rows
xml = JSON2XML_CSV(jsonData, csv); // Returns also XML string
res = csv.join('\r\n');
document.write('<textarea cols=100 rows=100>' + res + '</textarea>');
document.close();
*/
function JSON2XML_CSV(arr, xml, stack, prevHead) {
    if(!prevHead) prevHead = [1,,[]]; // row #, head string, recursive turn array
    if(!stack) stack = [];
    var res = [];
    var r = "";
    var row = stack.slice();
    var head = stack.slice();
    for(var a in arr) {
         try {
            if(arr[a] == null) { // Change null's to string
                    arr[a] = "null";
            }
            if(arr[a].toString().indexOf("[object") == 0
            || Array.isArray(arr[a])) {
                    if(isNaN(a)) {
                            r += '<' + a + '>\r\n';
                    } else {
                            r += '<No' + a + '>';
                    }
                    if(isNaN(a)) {
                            stack.push(a);
                    } else {
                            stack.push('#');
                    }
                    prevHead[2].push([]);
                    var sub = JSON2XML_CSV(arr[a], prevHead[2][prevHead[2].length-1], stack, prevHead).split('\n');
                    for(var b in sub) if(sub[b].length) r += sub[b];
                    if(isNaN(a)) { // Use text label
                            r += '</' + a + '>\r\n';
                    } else { // Prefix #
                            r += '</No' + a + '>\r\n';
                    }
                    stack.pop();
                    if(0 == stack.length) {
                        for (var b in prevHead[2]) {
                            for (var c in prevHead[2][b]) {
                                xml.push(prevHead[2][b][c]); // copy2 results in proper order
                            }
                            c = prevHead[2][b].length; // and clean
                            for (; c; c--) {
                                prevHead[2][b].pop();
                            }
                        }
                        c = prevHead[2].length;
                        for (; c; c--) {
                            prevHead[2].pop();
                        }
                    }
            } else {
                    head.push(a);
                    row.push(arr[a]);
                    r += '<' + a + ">" + arr[a] + '</' + a + '>';
            }
         } catch (e) {
                xml.push(e);
                return e;
         }
    }
    if(row.length != stack.length) {
            var h = head.join('\t');
            if(prevHead[1] != h) {
                    prevHead[0] = 1;
                    xml.push(h);
            }
            prevHead[1] = h;
            var numsNo = 0;
            for(var c in stack) {
                    if(row[c] == '#') numsNo++;
            }
            for(var c in stack) {
                    if(row[c] == '#') {
                        if(1 == numsNo--) {
                            row[c] = prevHead[0]++; // re-create numbered block for same headers - does not support multi level # yet
                        } else {
                            row[c] = ""; // no # except 4 deepest column
                        }
                    } else {
                        row[c] = '';
                    }
            }
            xml.push(row.join('\t'));
    }
    return r;
}
