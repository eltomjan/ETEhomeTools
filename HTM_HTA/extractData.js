(function () {
function responsive2CSV() {
   let srcNode = document.body;
   let els = [];
   let text_nodes = [];
   (function recursiveWalk(node) {
      if (node) {
         node = node.firstChild;
         while (node != null) {
            if (node.nodeType == 3) {
               if (node.parentElement.getClientRects().length) {
                  let text = node.nodeValue;
                  while (node.nextSibling && node.nextSibling.nodeType == 3) // merge split text nodes
                  {
                     node = node.nextSibling;
                     text += node.nodeValue;
                  }
                  let node2 = node;
                  while (node2.parentElement.childNodes.length <= 1) node2 = node2.parentElement; // find highest parent
                  while (node2.parentElement.innerText == text) node2 = node2.parentElement; // ignore invisible siblings text
                  if (node2.parentElement != srcNode && !text_nodes.includes(node2) && node2.nodeType != 3) // first time, no text
                     text_nodes.push(node2);
               }
            } else if (node.nodeType == 1) { recursiveWalk(node); }
            node = node.nextSibling;
         }
      }
   })(srcNode); // find highest text nodes
   els = [];
   const elDest = [];
   const leftIdx = {};
   for (let j = 0; j < text_nodes.length; j++) {
      const i = text_nodes[j];
      if (i.innerText == undefined) continue; // non-text objects svg, etc.
      let el = {
         x: i.offsetLeft, y: i.offsetTop,
         w: parseInt(i.getBoundingClientRect().width + 0.99), h: i.offsetHeight,
         text: i.innerText, ff: i.style.fontFamily, fs: i.style.fontSize, cssText: i.style.cssText
      };
      if (leftIdx[el.x] == undefined) leftIdx[el.x] = [el]; // create X index
      else {
         let newLine = 0;
         for (var k of leftIdx[el.x]) {
            if (k.y + k.h == el.y) { // join following lines in separate objects
               newLine++;
               break;
            }
         }
         if (newLine) {
            k.text += '\n' + el.text;
            k.h += el.h; // adjust size of joined element
            k.width = Math.max(k.width, el.width);
            continue;
         }
         leftIdx[el.x].push(el)
      }
      els.push(el);
   }
   els.sort(boxCmpMiddle); // 2D y -> x sort; centers of boxes compared
   let elMin = els[0];
   for (let i = 1; i < els.length; i++) {
      if (elDest[elDest.length - 1] !== elMin) elDest.push(elMin); // useless(?)
      elMin = els[i];
   }
   if (elDest[elDest.length - 1] !== elMin) elDest.push(elMin);
   const elList = [];
   for (let i = 0; i < elDest.length; i++) {
      elList.push([elDest[i].x, elDest[i].x + elDest[i].w, null, elDest[i].y, elDest[i].y + elDest[i].h, elDest[i].text]);
   }
   return XL();
   function boxCmp(a, b) {
      if (Math.abs(a.y - b.y) <= 1) {
         if (Math.abs(a.x - b.x) <= 1) return 0;
         else return a.x - b.x;
      } else return a.y - b.y;
   }
   function boxCmpMiddle(a, b) {
      if (Math.abs((a.y + a.h / 2) - (b.y + b.h / 2)) <= 1) {
         if (Math.abs(a.x - b.x) <= 1) return 0;
         else return a.x - b.x;
      } else return (a.y + a.h / 2) - (b.y + b.h / 2);
   }
   function escapeNL(cell) {
      if (cell.includes('\n')) {
         cell = '"' + cell + '"';
      }
      return cell;
   }
   function XL() {
      let xl = elList[0][5]; // text
      let xOld = elList[0];
      const lastXs = [elList];
      for (let i = 1; i < elList.length; i++) {
         if (elList[i][0] + 1 < xOld[1] || // x < oldX
            elList[i][3] > xOld[4]) { // y > oldY
            xl += '\n';
            while (elList[i][0][0] <= lastXs[lastXs.length - 1]) lastXs.pop();
            while (lastXs.length) {
               const e = lastXs.pop();
               if (e[1] < elList[i][0]) xl += '\t';
            }
            xl += escapeNL(elList[i][5]);
            lastXs.push(elList[i]);
         } else {
            xl += '\t' + escapeNL(elList[i][5]);
            lastXs.push(elList[i]);
         }
         xOld = elList[i];
      }
      return xl;
      // let ta;
      // if (document.getElementsByTagName('TEXTAREA').length === 2) ta = document.getElementsByTagName('TEXTAREA')[1];
      // else {
      //    ta = document.createElement('TEXTAREA');
      //    ta.setAttribute('style', 'width:100%;height:100%;position:absolute;left:0px;top:0px');
      //    ta.onblur = removeMe;
      //    document.body.appendChild(ta);
      // }
      // ta.value = xl;
      // ta.focus();
   }
   // function removeMe() {
   //    document.body.removeChild(event.target);
   // }
}
function saveForm() {
   f = document.forms, longest = 0; frm = 0;
   for (i = 0; i < f.length; i++)
      if (f[i].length > longest) { longest = f[i].length; frm = f[i] }
   values = {};
   for (a = 0; a < longest; a++) {
      el = frm[a];
      switch (el.type) {
         case "checkbox": values[el.name] = el.checked;
            break;
         case "radio":
            if (el.checked)
               values[el.name] = el.value;
            else if (values[el.name] === undefined)
               values[el.name] = false;
            break;
         case 'select-one': values[el.name] = el.selectedIndex < 0 ? -1 : el.options[el.selectedIndex].value; break;
         case 'select-multiple': values[el.name] = [];
            for (i = 0; i < el.options.length; i++) {
               if (el.options[i].selected)
                  values[el.name].push(el.options[i].value)
            }
            break;
         case 'fieldset': break;
         case 'button': break;
         case 'submit': break;
         case 'reset': break;
         case 'file': break;
            caseundefined: break;
         default: values[el.name] = el.value
      }
   }
   let w = window.open("");
   let data = JSON.stringify(values);
   let link = location.href.split('/').pop();
   link = link.replace(/\.\S+$/g, "")
   link = "<a href onclick=saveChanged(this) download='" + link + ".json" + "'>Download form data as JSON</a>" +
      " | <a href onclick=saveChangedCSV(this) download='" + link + ".csv" + "'>Responsive data as CSV</a>";
   let resp = responsive2CSV();
   w.document.write(link + "<textarea style='width:100%;height:calc(" + (resp ? "50% - 10px)'>" : "100% - 17px)'>") + data + "</textarea>" +
      (resp ? "<textarea style='width:100%;height:calc(50% - 10px)'>" + resp + "</textarea>" : ""));
   w.document.close();
   w.saveChanged = saveChanged;
   w.saveChangedCSV = saveChangedCSV;
}
function saveChanged(t) {
   let data = t.parentElement.getElementsByTagName("TEXTAREA")[0].value;
   t.href = "data:application/json;charset=utf-8," + encodeURIComponent(data);
}
function saveChangedCSV(t) {
   let data = t.parentElement.getElementsByTagName("TEXTAREA")[1].value;
   t.href = "data:application/json;charset=utf-8," + encodeURIComponent(data);
}
saveForm();
})()