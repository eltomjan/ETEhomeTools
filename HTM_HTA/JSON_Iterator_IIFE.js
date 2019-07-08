'use strict';
var JNode = (function (jsNode) {

    function JNode(_parent, _pred, _key, _value) {
        this.parent = _parent;
        this.pred = _pred;
        this.node = null;
        this.next = null;
        this.key = _key;
        this.value = _value;
    }

    return JNode;
})();

var JIterator = (function (json) {
    var root, current, maxLevel = -1;

    function JIterator(json, parent) {
        if (parent === undefined) parent = null;
        var pred = null, localCurrent;
        for (var child in json) {
            var obj = json[child] instanceof Object;
            if (json instanceof Array) child = parseInt(child); // non-associative array
            if (!root) root = localCurrent = new JNode(parent, null, child, json[child]);
            else {
                localCurrent = new JNode(parent, pred, child, obj ? ((json[child] instanceof Array) ? [] : {}) : json[child]);
            }
            if (pred) pred.next = localCurrent;
            if (parent && parent.node == null) parent.node = localCurrent;
            pred = localCurrent;
            if (obj) {
                var memPred = pred;
                JIterator(json[child], pred);
                pred = memPred;
            }
        }
        if (this) {
            current = root;
            this.Level = 0;
        }
    }

    JIterator.prototype.Current = function () { return current; }
    JIterator.prototype.SetCurrent = function (newCurrent) { current = newCurrent; }
    JIterator.prototype.Parent = function () {
        var retVal = current.parent;
        if (retVal == null) return false;
        this.Level--;
        return current = retVal;
    }
    JIterator.prototype.Pred = function () {
        var retVal = current.pred;
        if (retVal == null) return false;
        return current = retVal;
    }
    JIterator.prototype.Node = function () {
        var retVal = current.node;
        if (retVal == null) return false;
        this.Level++;
        return current = retVal;
    }
    JIterator.prototype.Next = function () {
        var retVal = current.next;
        if (retVal == null) return false;
        return current = retVal;
    }
    JIterator.prototype.Key = function () { return current.key; }
    JIterator.prototype.KeyDots = function () { return (typeof (current.key) == "number") ? "" : (current.key + ':'); }
    JIterator.prototype.Value = function () { return current.value; }
    JIterator.prototype.Reset = function () {
        current = root;
        this.Level = 0;
    }
    JIterator.prototype.RawPath = function () {
        var steps = [], level = current;
        do {
            if (level != null && level.value instanceof Object) {
                steps.push(level.key + (level.value instanceof Array ? "[]" : "{}"));
            } else {
                if (level != null) steps.push(level.key);
                else break;
            }
            level = level.parent;
        } while (level != null);
        var retVal = "";
        retVal = steps.reverse();
        return retVal;
    }
    JIterator.prototype.Path = function () {
        var steps = [], level = current;
        do {
            if (level != null && level.value instanceof Object) {
                var size = 0;
                var items = level.node;
                if (typeof (level.key) == "number") steps.push('[' + level.key + ']');
                else {
                    while (items) {
                        size++;
                        items = items.next;
                    }
                    var type = (level.value instanceof Array ? "[]" : "{}");
                    var prev = steps[steps.length - 1];
                    if (prev && prev[0] == '[') {
                        var last = prev.length - 1;
                        if (prev[last] == ']') {
                            last--;
                            if (!isNaN(prev.substr(1, last))) {
                                steps.pop();
                                size += '.' + prev.substr(1, last);
                            }
                        }
                    }
                    steps.push(level.key + type[0] + size + type[1]);
                }
            } else {
                if (level != null) {
                    if (typeof (level.key) == "number") steps.push('[' + level.key + ']');
                    else steps.push(level.key);
                }
                else break;
            }
            level = level.parent;
        } while (level != null);
        var retVal = "";
        retVal = steps.reverse();
        return retVal;
    }
    JIterator.prototype.DepthFirst = function () {
        if (current == null) return 0; // exit sign
        if (current.node != null) {
            current = current.node;
            this.Level++;
            if (maxLevel < this.Level) maxLevel = this.Level;
            return 1; // moved down
        } else if (current.next != null) {
            current = current.next;
            return 2; // moved right
        } else {
            while (current != null) {
                if (current.next != null) {
                    current = current.next;
                    return 3; // returned up & moved next
                }
                this.Level--;
                current = current.parent;
            }
        }
        return 0; // exit sign
    }
    JIterator.prototype.BreadthFirst = function () {
        if (current == null) return 0; // exit sign
        if (current.next) {
            current = current.next;
            return 1; // moved right
        } else if (current.parent) {
            var level = this.Level, point = current;
            while (this.DepthFirst() && level != this.Level);
            if (current) return 2; // returned up & moved next
            do {
                this.Reset();
                level++;
                while (this.DepthFirst() && level != this.Level);
                if (current) return 3; // returned up & moved next
            } while (maxLevel >= level);
            return current != null ? 3 : 0;
        } else if (current.node) {
            current = current.node;
            return 3;
        } else if (current.pred) {
            while (current.pred) current = current.pred;
            while (current && !current.node) current = current.next;
            if (!current) return null;
            else return this.DepthFirst();
        }
    }
    JIterator.prototype.ReadArray = function () {
        var retVal = {};
        var item = current;
        do {
            if (item.value instanceof Object) {
                if (item.value.length == 0) retVal[item.key] = item.node;
                else retVal[item.key] = item;
            } else retVal[item.key] = item.value;
            item = item.next;
        } while (item != null);
        return retVal;
    }
    JIterator.prototype.FindKey = function (key) {
        var pos = current;
        while (current && current.key != key) this.DepthFirst();
        if (current.key == key) {
            var retVal = current;
            current = pos;
            return retVal;
        } else {
            current = pos;
            return null;
        }
    }

    return JIterator;
})();
