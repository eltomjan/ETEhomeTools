const fs = require('fs');
var src = fs.readFileSync(__filename).toString();
src = 'javascript:' + src.split('/* st' + 'art */\r\n')[1];
src = src.replace(/\n\s+/g, '\n')
    .replace(/;\r\n/g, ';')
    .replace(/{\r\n/g, '{')
    .replace(/}\r\n/g, '}');
fs.writeFileSync(__filename.split('.')[0] + '_min.js', src);
/* start */
(function () {
    const src = document.getElementsByClassName('textLayer')[0].children;
    let els = [];
    const elDest = [];
    for (let j = 0; j < src.length; j++) {
        const i = src[j];
        if (i.className === 'endOfContent') continue;
        els.push({ x: i.offsetLeft, y: i.offsetTop, w: parseInt(i.getBoundingClientRect().width + 0.99), h: i.offsetHeight, text: i.innerText, ff: i.style.fontFamily, fs: i.style.fontSize, cssText: i.style.cssText });
    }
    els.sort(boxCmp);
    let elMin = els[0];
    for (let i = 1; i < els.length; i++) {
        if (elMin.x + elMin.w + 1 >= els[i].x && elMin.y === els[i].y && elMin.h === els[i].h && elMin.ff === els[i].ff && elMin.fs === els[i].fs) {
            elMin.text += els[i].text;
            elMin.w = els[i].x + els[i].w - elMin.x;
            if (elDest[elDest.length - 1] !== elMin) elDest.push(elMin);
            continue;
        }
        if (elDest[elDest.length - 1] !== elMin) elDest.push(elMin);
        elMin = els[i];
    }
    els = document.getElementsByClassName('textLayer')[0];
    while (els.lastChild) els.removeChild(els.lastChild);
    const elList = [];
    const uqIdx = { x: [], y: [] };
    for (let i = 0; i < elDest.length; i++) {
        const o = document.createElement('INPUT');
        o.value = elDest[i].text;
        o.setAttribute('style', elDest[i].cssText + 'width:' + elDest[i].w + 'px;position:absolute;');
        els.appendChild(o);
        elList.push([elDest[i].x, elDest[i].x + elDest[i].w, o, elDest[i].y, elDest[i].y + elDest[i].h, elDest[i].text]);
        if (uqIdx.x.indexOf(elDest[i].x) < 0) uqIdx.x.push(elDest[i].x);
        if (uqIdx.y.indexOf(elDest[i].y) < 0) uqIdx.y.push(elDest[i].y);
    }
    uqIdx.x.sort(num);
    uqIdx.y.sort(num);
    document.body.ondblclick = XL;
    document.body.onkeydown = moveEl;
    const s = document.createElement('STYLE');
    s.innerHTML = 'input{padding:0px;border:1px solid #e0e0e0;}input:focus{background-color:red;}';
    document.head.appendChild(s);
    src[0].focus();
    function num (a, b) { return a - b; }
    function boxCmp (a, b) {
        if (Math.abs(a.y - b.y) <= 1) {
            if (Math.abs(a.x - b.x) <= 1) return 0;
            else return a.x - b.x;
        } else return a.y - b.y;
    }
    var me;
    function moveEl (event) {
        const el = event.target;
        if (event.target.tagName !== 'INPUT') return;
        let i = 0;
        while (i < elList.length && elList[i][2] !== el) i++;
        me = elList[i];
        const flw = [];
        if (event.key.indexOf('PageDown') > -1) {
            if (++i < elList.length) elList[i][2].focus();
            return;
        } else if (event.key.indexOf('PageUp') > -1) {
            if (i) elList[--i][2].focus();
            return;
        } else if (event.key.indexOf('Left') > -1) {
            while (i--) {
                if (elList[i][0] < me[0] && me[3] <= elList[i][4]) flw.push(elList[i]);
                else break;
            }
            flw.sort(distanceX);
        } else if (event.key.indexOf('Right') > -1) {
            while (++i < elList.length) {
                if (elList[i][0] > me[0] && me[4] >= elList[i][3]) flw.push(elList[i]);
                else break;
            }
            flw.sort(distanceX);
        } else if (event.key.indexOf('Down') > -1) {
            while (++i < elList.length) if (elList[i][3] > me[3]) flw.push(elList[i]);
            flw.sort(distance);
        } else if (event.key.indexOf('Up') > -1) {
            while (i--) if (elList[i][3] < me[3]) flw.push(elList[i]);
            flw.sort(distance);
        }
        if (!flw.length) return;
        flw[0][2].focus();
    }
    function distance (a, b) {
        const ac0 = Math.abs(me[0] - a[0]) + Math.abs(me[3] - a[3]);
        const ac1 = Math.abs(me[1] - a[1]) + Math.abs(me[4] - a[4]);
        const bc0 = Math.abs(me[0] - b[0]) + Math.abs(me[3] - b[3]);
        const bc1 = Math.abs(me[1] - b[1]) + Math.abs(me[4] - b[4]);
        return Math.min(ac0, ac1) - Math.min(bc0, bc1);
    }
    function distanceX (a, b) {
        const ac = Math.abs(me[0] - a[0]);
        const bc = Math.abs(me[0] - b[0]);
        return ac - bc;
    }
    function XL () {
        const xl = [];
        for (let y = 0; y < uqIdx.y.length; y++) {
            const r = [];
            for (let x = 0; x < uqIdx.x.length; x++)r.push([]);
            xl.push(r);
        }
        for (let i = 0; i < elList.length; i++) {
            const el = elList[i];
            xl[uqIdx.y.indexOf(el[3])][uqIdx.x.indexOf(el[0])] = el[2].value;
        }
        let str = '';
        for (let y = 0; y < uqIdx.y.length; y++) {
            str += xl[y].join('\t').replace(/\t+$/, '') + '\n';
        }
        let ta;
        if (document.getElementsByTagName('TEXTAREA').length === 2) ta = document.getElementsByTagName('TEXTAREA')[1];
        else {
            ta = document.createElement('TEXTAREA');
            ta.setAttribute('style', 'width:100%;height:100%;position:absolute;left:0px;top:0px');
            ta.onblur = removeMe;
            document.body.appendChild(ta);
        }
        ta.value = str;
        ta.focus();
    }
    function removeMe () {
        document.body.removeChild(event.target);
    }
})();
