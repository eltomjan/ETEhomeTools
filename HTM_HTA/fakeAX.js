/*
var nodeJS = {};
var document, Enumerator, ActiveXObject;
if (!this.window) {
  document = {
    location: { pathname: '/' + __filename.replace(/\\/g, '/') }
  };
  var AX = require('./fakeAX')(nodeJS);
  for(let i in AX) eval(i + "= AX." + i);
}
*/
module.exports = function(nodeJS) {
nodeJS.fs = require('fs');
nodeJS.path = require('path');
Enumerator = (function () {

    function Enumerator(data) {
        this.list = data;
        this.items = [];
        for (var i in data) { this.items.push(i); }
        this.pos = 0;
    }

    Enumerator.prototype = {
        item: function () {
            return this.list[this.items[this.pos]];
        },
        atEnd: function () {
            return this.pos >= this.items.length - 1;
        },
        moveNext: function () {
            if (this.pos < this.items.length) this.pos++;
            else return false;
            return true;
        }
    }

    return Enumerator;
})();
ActiveXObject = (function () {
    var Scripting_FileSystemObject = (function Scripting_FileSystemObject() {
        var Scripting_Folder = (function Scripting_Folder() {

            function Scripting_Folder(f) {
                this.path = f;
            }
            Scripting_Folder.prototype = {
                get SubFolders() {
                    var retVal = [], path = this.path;
                    const dirs = nodeJS.fs.readdirSync(this.path, { withFileTypes: true });
                    for (var i in dirs) {
                        if (dirs[i].isDirectory())
                            retVal.push(new Scripting_Folder(nodeJS.path.join(path, dirs[i].name)));
                    }
                    return retVal;
                },
                get Name() {
                    return this.path.substr(this.path.lastIndexOf('\\') + 1);
                },
                get Files() {
                    var retVal = [], path = this.path;
                    const dirs = nodeJS.fs.readdirSync(this.path, { withFileTypes: true });
                    for (var i in dirs) {
                        if (!dirs[i].isDirectory())
                            retVal.push(new Scripting_Folder(nodeJS.path.join(path, dirs[i].name)));
                    }
                    return retVal;
                },
                get Path() { return this.path; }
            }

            return Scripting_Folder;
        })();
        var Scripting_File = (function Scripting_File() {

            function Scripting_File(f) {
                this.path = f;
            }
            Scripting_File.prototype = {
                get ParentFolder() {
                    var p = this.path.substr(0, this.path.lastIndexOf('\\'));
                    return new Scripting_File(p);
                },
                get Path() { return this.path; }
            }

            return Scripting_File;
        })();

        Scripting_FileSystemObject.prototype = {
            GetFile: function (f) {
                return new Scripting_File(f);
            },
            GetFolder: function (s) {
                return new Scripting_Folder(s);
            },
            OpenTextFile: function (s, mode) { // https://docs.microsoft.com/en-us/office/vba/language/reference/user-interface-help/opentextfile-method
                const translate = { // https://nodejs.org/api/fs.html#fs_file_system_flags
                    1: 'r',
                    2: 'w',
                    8: 'a'
                }
                return new NodeFile(nodeJS.fs.openSync(s, translate[mode]));
            }
        }

        return Scripting_FileSystemObject;
    })();
    var NodeFile = (function () {
        function NodeFile(no) {
            this.fd = no;
        }

        NodeFile.prototype = {
            ReadAll: function () {
                var buf = nodeJS.fs.readFileSync(this.fd);
                return buf.toString();
            },
            Close: function () {
                return nodeJS.fs.closeSync(this.fd);
            }
        }

        return NodeFile;
    })();
    var WScript_Shell = (function WScript_Shell() {
        const execSync = require('child_process').execSync;

        WScript_Shell.prototype.Exec = function (s) {
            return {
                StdOut: {
                    ReadAll: function () { return execSync(s).toString(); }
                }
            }
        }

        return WScript_Shell;
    })();

    function ActiveXObject(s) {
        switch (s) {
            case 'Scripting.FileSystemObject': {
                this.__proto__ = Scripting_FileSystemObject.prototype;
                break;
            };
            case 'WScript.Shell': {
                this.__proto__ = WScript_Shell.prototype;
            }
        }
    }

    return ActiveXObject;
})();

return {
    ActiveXObject:ActiveXObject,
    Enumerator: Enumerator
}}