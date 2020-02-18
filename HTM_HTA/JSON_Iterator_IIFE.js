'use strict';
var JNode = (function (jsNode) {
    function JNode (_json, _parent, _pred, _key, _value) {
        this.parent = _parent;
        this.pred = _pred;
        this.node = null;
        this.next = null;
        this.key = _key;
        this.value = _value;
        this.json = _json;
    }
    JNode.prototype = {
        get HasOwnKey () { return this.key && (this.key.constructor !== Number); },
        get HasStringValue () { return this.value.constructor === String; }
    };
    return JNode;
})();
var JIterator = (function (json) {
    var JNodePrivates = (function (parent) {
        function JNodePrivates () {
            this.root = null;
            this.current = null;
        }
        JNodePrivates.prototype = {
            get Root () {
                return this.root;
            },
            setRoot: function (newRoot) {
                return (this.root = newRoot);
            },
            get Current () {
                return this.current;
            },
            setCurrent: function (newCurrent) {
                return (this.current = newCurrent);
            }
        };
        return JNodePrivates;
    })();
    var maxLevel = -1;
    function JIterator (json, parent) {
        if (this._privates === undefined) this._privates = new JNodePrivates();
        if (parent === undefined) parent = null;
        var pred = null, localCurrent;
        for (var child in json) {
            var obj = json[child] instanceof Object;
            if (json instanceof Array) child = parseInt(child); // non-associative array
            if (!this._privates.Root) this._privates.setRoot(localCurrent = new JNode(json, parent, null, child, json[child]));
            else {
                localCurrent = new JNode(json[child], parent, pred, child, obj ? ((json[child] instanceof Array) ? [] : {}) : json[child]);
            }
            if (pred) pred.next = localCurrent;
            if (parent && parent.node == null) parent.node = localCurrent;
            pred = localCurrent;
            if (obj) {
                var memPred = pred;
                JIterator.call(this, json[child], pred);
                pred = memPred;
            }
        }
        if (!this._privates.Current && this._privates.Root) this._privates.setCurrent(this._privates.Root);
    }
    JIterator.prototype = {
        // Public Getters
        get Current () { return this._privates.Current; },
        SetCurrent: function (newCurrent) { return this._privates.setCurrent(newCurrent); },
        get Path () {
            var steps = [], level = this._privates.Current;
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
            steps.forEach(function (el, i) {
                if (!isNaN(el)) steps[i] = '[' + el + ']';
            });
            return steps.reverse().join('.');
        },
        // Public Setters
        set current (value) {
            console.log('Use SetCurrent(' + value + ') !');
            throw 'Access to current Denied !';
        },
        // Public methods
        Parent: function () {
            var retVal = this._privates.Current.parent;
            if (retVal == null) return false;
            this.Level--;
            return this._privates.setCurrent(retVal);
        },
        Pred: function () {
            var retVal = this._privates.Current.pred;
            if (retVal == null) return false;
            return this._privates.setCurrent(retVal);
        },
        Node: function () {
            var retVal = this._privates.Current.node;
            if (retVal == null) return false;
            this.Level++;
            return this._privates.setCurrent(retVal);
        },
        Next: function () {
            var retVal = this._privates.Current.next;
            if (retVal == null) return false;
            return this._privates.setCurrent(retVal);
        },
        get Key () {
            if (this._privates.Current) return this._privates.Current.key;
            return undefined;
        },
        KeyDots: function () { return (!this.HasOwnKey) ? '' : (this._privates.Current.key + ':'); },
        get Value () {
            if (this._privates.Current) return this._privates.Current.value;
            return undefined;
        },
        Reset: function () {
            this._privates.setCurrent(this._privates.Root);
            this.Level = 0;
        },
        RawPath: function () {
            var steps = [], level = this._privates.Current;
            do {
                if (level != null && level.value instanceof Object) {
                    steps.push(level.key + (level.value instanceof Array ? '[]' : '{}'));
                } else {
                    if (level != null) steps.push(level.key);
                    else break;
                }
                level = level.parent;
            } while (level != null);
            var retVal = '';
            retVal = steps.reverse();
            return retVal;
        },
        DepthFirst: function () {
            if (this._privates.Current == null) return 0; // exit sign
            if (this._privates.Current.node != null) {
                this._privates.setCurrent(this._privates.Current.node);
                this.Level++;
                if (maxLevel < this.Level) maxLevel = this.Level;
                return 1; // moved down
            } else if (this._privates.Current.next != null) {
                this._privates.setCurrent(this._privates.Current.next);
                return 2; // moved right
            } else {
                while (this._privates.Current != null) {
                    if (this._privates.Current.next != null) {
                        this._privates.setCurrent(this._privates.Current.next);
                        return 3; // returned up & moved next
                    }
                    this.Level--;
                    this._privates.setCurrent(this._privates.Current.parent);
                }
            }
            return 0; // exit sign
        },
        BreadthFirst: function () {
            if (this._privates.Current == null) return 0; // exit sign
            if (this._privates.Current.next) {
                this._privates.setCurrent(this._privates.Current.next);
                return 1; // moved right
            } else if (this._privates.Current.parent) {
                var level = this.Level;
                while (this.DepthFirst() && level !== this.Level);
                if (this._privates.Current) return 2; // returned up & moved next
                do {
                    this.Reset();
                    level++;
                    while (this.DepthFirst() && level !== this.Level);
                    if (this._privates.Current) return 3; // returned up & moved next
                } while (maxLevel >= level);
                return this._privates.Current != null ? 3 : 0;
            } else if (this._privates.Current.node) {
                this._privates.setCurrent(this._privates.Current.node);
                return 3;
            } else if (this._privates.Current.pred) {
                while (this._privates.Current.pred) this._privates.setCurrent(this._privates.Current.pred);
                while (this._privates.Current && !this._privates.Current.node) this._privates.setCurrent(this._privates.Current.next);
                if (!this._privates.Current) return null;
                else return this.DepthFirst();
            }
        },
        ReadArray: function () {
            var retVal = {};
            var item = this._privates.Current;
            do {
                if (item.value instanceof Object) {
                    if (item.value.length === 0) retVal[item.key] = item.node;
                    else retVal[item.key] = item;
                } else retVal[item.key] = item.value;
                item = item.next;
            } while (item != null);
            return retVal;
        },
        FindKey: function (key) {
            var pos = this._privates.Current;
            while (this._privates.Current && this._privates.Current.key !== key) this.DepthFirst();
            if (this._privates.Current.key === key) {
                var retVal = this._privates.Current;
                this._privates.setCurrent(pos);
                return retVal;
            } else {
                this._privates.setCurrent(pos);
                return null;
            }
        },
        FindPair: function (key, value, move2) {
            var pos = this._privates.current;
            while (this._privates.current) {
                if (this._privates.current.key === key && this._privates.current.value === value) {
                    break;
                } else this.DepthFirst();
            }
            if (move2) return this._privates.current;
            var retVal = this._privates.current;
            this.SetCurrent(pos);
            return retVal;
        },
        // Debug info methods
        PathDetails: function (brief) {
            var steps = [], level = this._privates.Current;
            do {
                if (level != null && level.value instanceof Object) {
                    var size = 0;
                    var items = level.node;
                    if (!level.HasOwnKey && !brief) steps.push('[' + level.key + ']');
                    else {
                        if(brief) {
                            if (level.HasOwnKey) steps.push(level.key);
                        } else {
                            while (items) {
                                size++;
                                items = items.next;
                            }
                            var type = (level.value instanceof Array ? '[]' : '{}');
                            var prev = steps[steps.length - 1];
                            if (prev && prev[0] === '[') {
                                var last = prev.length - 1;
                                if (prev[last] === ']') {
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
                        if (!level.HasOwnKey) steps.push('[' + level.key + ']');
                        else steps.push(level.key);
                    }
                    else break;
                }
                level = level.parent;
            } while (level != null);
            var retVal = "";
            retVal = steps.reverse();
            return retVal;
        },
        Move2path: function (json) {
            var nd = this._privates.current;
            var pth = [nd];
            while (nd.parent) {
                nd = nd.parent;
                pth.push(nd);
            }
            pth.reverse();
            for (var x in pth) json = json[pth[x].key];
            return json;
        },
        CreateNode: function (json, key, value) {
            var current = this._privates.Current;
            if (current) {
                current.node = new JNode(json, current, current.pred, key, value);
                if (isNaN(key)) json[key] = value;
                return current.node;
            } else {
                current = new JNode(json, null, null, key, value);
                json[key] = value;
                this._privates.setCurrent(current);
                this._privates.setRoot(current);
                return current;
            }
        }
    };
    return JIterator;
})();
