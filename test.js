const kanaconv = require('./build/Release/kanaconv');
const t = require('./test.json');
const assert = require('assert');
const util = require('util');

t.forEach(function(c, i) {
    const actual = kanaconv.convert(c[1], c[0]);
    const expected = c[2];
    console.log(`test case[${i}] ==> \n   opts:${c[0]} actual:${util.inspect(actual)} expected:${util.inspect(expected)}`);
    assert.strictEqual(actual, expected);
})

//console.log(kanaconv.hello()); // hello, world が出力される

// r: 「全角英字」を「半角」に変換します。
/*
console.log('結果:', kanaconv.convert("０１２３　:ａｂｃ/ＡＢＣ\n", 'r'));

// n: 「全角数字」を「半角」に変換します。
console.log('結果:', kanaconv.convert("０１２３　:ａｂｃ/ＡＢＣ\n", 'n'));

// a: 「全角英数字」を「半角」に変換します。
console.log('結果:', kanaconv.convert("０１２３　:ａｂｃ/ＡＢＣ\n", 'a'));

// s: 「全角スペース」を「半角」に変換します。
console.log('結果:', kanaconv.convert("０１２３　:ａｂｃ/ＡＢＣ\n", 's'));

// k: 「全角カタカナ」を「半角」に変換します。
console.log('結果:', kanaconv.convert("ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ\n", 'k'));

// c: 「全角カタカナ」を「全角ひらがな」に変換します。
console.log('結果:', kanaconv.convert("アヽスヾメアヽスヾメ\n", 'c'));
*/
