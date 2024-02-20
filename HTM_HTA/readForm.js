(function(){
function setFormData(values) {
    f = document.forms, longest = 0; frm = 0;
    for (i = 0; i < f.length; i++)
        if (f[i].length > longest) {
            longest = f[i].length; frm = f[i]
        }
    for (v in values) {
        el = frm[v]; val = values[v];
        if (el.constructor == HTMLCollection) {
            for (p = 0; p < el.length; p++) {
                el[p].checked = (el[p].value === val)
            }
        } else switch (el.type) {
            case "checkbox": el.checked = val;
                break;
            case 'select-one':
                if (val < 0) el.selectedIndex = val;
                else {
                    for (o = 0; o < el.options.length; o++) {
                        if (el.options[o].value == val) el.selectedIndex = o
                    }
                }
                break;
            case 'select-multiple':
                for (o = 0; o < el.options.length; o++) {
                    el.options[o].selected = (val.indexOf(el.options[o].value) > -1)
                }
                break;
            default: el.value = val
        }
    }
}
f = document.createElement("input");
f.type = "file";
f.onchange = (function () {
    reader = new FileReader();
    reader.onload = (function (theFile) {
        return function (e) {
            json = JSON.parse(e.target.result);
            setFormData(json);
            return;
        }
    })(event.srcElement.files[0]);
    reader.readAsText(event.srcElement.files[0]);
});
document.body.prepend(f);
f.focus();
})()