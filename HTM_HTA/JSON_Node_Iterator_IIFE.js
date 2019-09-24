// JSON_Node_Iterator_IIFE.js
'use strict';
var JNode = (function (jsNode) {

    function JNode(json, parent, pred, key, obj, fill) {
        var node, pred = null;
        if (parent === undefined) {
            parent = null;
        } else if (fill) {
            this.parent = parent;
            this.pred = pred;
            this.node = null;
            this.next = null;
            this.key = key;
            this.obj = obj;
            return this;
        }
        var current;
        var parse = (json instanceof Array);
        for (var child in json) {
            if (parse) child = parseInt(child);
            var sub = json[child];
            node = new JNode(null, parent, pred, child, sub, true);
            if (pred) {
                pred.next = node;
                node.pred = pred;
            }
            if (!current) current = node;
            pred = node;
        }
        return current;
    }

    JNode.prototype = {
        get hasNode() {
            if (this.node) return this.node;
            return (this.obj instanceof Object);
        },
        get hasOwnKey() { return this.key && (typeof this.key != "number"); },
        get level() {
            var level = 1, i = this;
            while(i = i.parent) level++;
            return level;
        },
        GoDown: function() {
            if (!this.node && this.obj instanceof Object) {
                this.node = new JNode(this.obj, this);
            }
            return this.node;
        },
        Stringify: function() { // Raw test stringify - #s are taken same as strings
            var res;
            if (typeof this.key == "number") {
                res = '[';
                var i = this;
                do {
                    if (i.node) res += i.node.Stringify();
                    else res += "undefined";
                    i = i.next;
                    if (i) res += ','
                } while(i);
                res += ']';
            } else {
                res = '{' + '"' + this.key + '":';
                res += (this.node?this.node.Stringify():this.hasNode?"undefined":'"'+this.obj+'"');
                var i = this;
                while (i = i.next) {
                    res += ',' + '"' + i.key + '":';
                    if (i.node) res += i.node.Stringify();
                    else {
                        if (i.obj instanceof Object) res += "undefined";
                        else res += '"' + i.obj + '"';
                    }
                };
                res += '}';
            }
            return res;
        },
        DepthFirst: function () {
            if (this == null) return 0; // exit sign
            if (this.node != null || this.obj instanceof Object) {
                return this.Down(); // moved down
            } else if (this.next != null) {
                return this.next;// moved right
            } else {
                var i = this;
                while (i != null) {
                    if (i.next != null) {
                        return i.next; // returned up & moved next
                    }
                    i = i.parent;
                }
            }
            return 0; // exit sign
        }
    }

    return JNode;
})();
