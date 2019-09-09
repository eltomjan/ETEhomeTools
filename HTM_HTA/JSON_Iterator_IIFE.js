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
    JNode.prototype = {
        HasOwnKey: function () { return this.key && (typeof this.key != "number"); },
        HasStringValue: function () { return !(this.value instanceof Object); }
    }

    return JNode;
})();

var JIterator = (function (json) {
    var root, current, maxLevel = -1;

    function JIterator(json, parent) {
        if (parent === undefined) parent = null;
        var objs = [];
        do {
            var pred = null, localCurrent;
            for (var child in json) {
                var isObject = json[child] instanceof Object;
                if (json instanceof Array) child = parseInt(child); // non-associative array
                if (!root) root = localCurrent = new JNode(parent, null, child, json[child]);
                else {
                    localCurrent = new JNode(parent, pred, child, isObject ? ((json[child] instanceof Array) ? [] : {}) : json[child]);
                }
                if (pred) pred.next = localCurrent;
                if (parent && parent.node == null) parent.node = localCurrent;
                pred = localCurrent;
                if (isObject) { // remember parent and object for recursion
                    objs.push(pred);
                    objs.push(json[child]);
                }
            }
            if (objs.length) { // get next object and parent for recursion
                json = objs.pop();
                parent = objs.pop();
                continue;
            } else objs = false;
        } while (objs);
        if (this) {
            current = root;
            this.Level = 0;
        }
    }

    JIterator.prototype = {
        // Public Getters
        get current() { return current; },
        get key() { return current.key; },
        get value() { return current.value; },
        get path() {
            var steps = [], level = current;
            do {
                if (level != null && level.value instanceof Object) {
                    if (level.value instanceof Array) {
                        if (steps.length > 0) {
                            steps.push(level.key + '[' + steps.pop() + ']');
                        } else {
                            steps.push(level.key);
                        }
                    } else {
                        steps.push(level.key);
                    }
                } else {
                    if (level != null) steps.push(level.key);
                    else break;
                }
                level = level.parent;
            } while (level != null);
            return steps.reverse().join('.');
        },
        get hasOwnKey() { return current.key && (typeof current.key != "number"); },
        get hasStringValue() { return !(current.value instanceof Object); },

        // Public Setters
        set current(value) {
            console.log("Use SetCurrent(" + value + ") !");
            throw "Access to current Denied !";
        },

        // Public methods
        SetCurrent: function (newCurrent) {
            current = newCurrent;
            this.Level = 0;
            while(newCurrent = newCurrent.parent) this.Level++;
        },
        Parent: function () {
            var retVal = current.parent;
            if (retVal == null) return false;
            this.Level--;
            return current = retVal;
        },
        Pred: function () {
            var retVal = current.pred;
            if (retVal == null) return false;
            return current = retVal;
        },
        Node: function () {
            var retVal = current.node;
            if (retVal == null) return false;
            this.Level++;
            return current = retVal;
        },
        Next: function () {
            var retVal = current.next;
            if (retVal == null) return false;
            return current = retVal;
        },
        KeyDots: function () { return (typeof (current.key) == "number") ? "" : (current.key + ':'); },
        Reset: function () {
            current = root;
            this.Level = 0;
        },
        DepthFirst: function () {
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
        },
        BreadthFirst: function () {
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
                this.Level++;
                return 3;
            } else if (current.pred) {
                while (current.pred) current = current.pred;
                while (current && !current.node) current = current.next;
                if (!current) return null;
                else return this.DepthFirst();
            }
        },
        ReadArray: function (current2JSON) {
            var retVal = current.parent && current.parent.value instanceof Array ? [] : {};
            var empty = true;
            var item = current;
            do {
                if (item.value instanceof Object) {
                    if (current2JSON) {
                        empty = false;
                        var oldCurrent = current;
                        current = item.node;
                        retVal[item.key] = this.ReadArray(current2JSON);
                        current = oldCurrent;
                    }
                } else {
                    empty = false;
                    retVal[item.key] = item.value;
                }
                item = item.next;
            } while (item != null);
            if (empty) return []; // empty Array instead of empty Object
            return retVal;
        },
        FindKey: function (key, move2) {
            var pos = current;
            while (current) {
                if (current.key == key) break;
                else this.DepthFirst();
            } 
            if (move2) return current;
            var retVal = current;
            this.SetCurrent(pos);
            return retVal;
        },
        FindPair: function (key, value, move2) {
            var pos = current;
            while (current) {
                if (current.key == key && current.value == value) {
                    break;
                } else this.DepthFirst();
            }
            if (move2) return current;
            var retVal = current;
            this.SetCurrent(pos);
            return retVal;
        },
    
        // Debug info methods
        PathDetails: function (brief) {
            var steps = [], level = current;
            do {
                if (level != null && level.value instanceof Object) {
                    var size = 0;
                    var items = level.node;
                    if (typeof (level.key) == "number" && !brief) steps.push('[' + level.key + ']');
                    else {
                        if(brief) {
                            if (typeof (level.key) != "number") steps.push(level.key);
                        } else {
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
    }

    return JIterator;
})();
