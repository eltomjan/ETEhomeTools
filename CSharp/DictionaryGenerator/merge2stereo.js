/* Setup (node.js part):
  npm i -g wavefile
*/
const path = require('path');
const fs = require('fs');
const WaveFile = require(path.join(process.env.APPDATA, 'npm/node_modules', 'wavefile')).WaveFile;

function main () {
    let names; let wavs; const info = [{}, {}];
    const len2 = [0, 0];
    for (let i = 1000; i < 1635; i++) {
        names = [
            'a' + i.toString().substr(1) + '.wav',
            'c' + i.toString().substr(1) + '.wav'
        ];
        wavs = [
            new WaveFile(fs.readFileSync(path.join(__dirname, names[0]))),
            new WaveFile(fs.readFileSync(path.join(__dirname, names[1])))
        ];
        getInfo(wavs[0], info[0]);
        getInfo(wavs[1], info[1]);
        const data = adjust(info);
        const wav = new WaveFile();
        wav.fromScratch(2, 19456, '16', data);
        console.log(names[0] + '\t' + info[0].len + '\t' + names[1] + '\t' + info[1].len);
        len2[0] += info[0].len;
        len2[1] += info[1].len;
        fs.writeFileSync(path.join(__dirname, 'out', i.toString().substr(1) + '.wav'), wav.toBuffer());
    }
    console.log(len2);
}
main();

function adjust (info) {
    const mults = [
        65536 / (info[0].max - info[0].min),
        (65536 / (info[1].max - info[1].min)) * 0.25
    ];
    const lens = [info[0].len, info[1].len];
    const ret = [[], []];
    const halfMax = Math.max(lens[0], lens[1]) / 2;
    const poss = [
        parseInt(info[0].start - halfMax + lens[0] / 2 + 0.99),
        parseInt(info[1].start - halfMax + lens[1] / 2 + 0.99)
    ];
    const mgLen = Math.max(lens[0], lens[1]);

    for (let i = 0; i < mgLen; i++) {
        let j = i + poss[0];
        if (j > 0 && j <= info[0].end) ret[0].push((info[0].data[j] - info[0].avg) * mults[0]);
        else ret[0].push(0);
        j = i + poss[1];
        if (j > 0 && j <= info[1].end) ret[1].push((info[1].data[j] - info[1].avg) * mults[1]);
        else ret[1].push(1);
    }
    return ret;
}

function getInfo (w, r) {
    r.data = w.getSamples(false);
    let i = 0;
    while (!r.data[i]) i++;
    r.start = i;
    i = r.data.length;
    while (!r.data[--i]);
    r.end = i;
    r.len = r.end - r.start + 1;
    r.min = 0;
    r.max = 0;
    let sum = 0;
    for (i = r.start; i < r.end; i++) {
        const val = r.data[i];
        if (val < r.min) r.min = val;
        else if (val > r.max) r.max = val;
        sum += val;
    }
    r.avg = sum / (r.end - r.start + 1);
}
