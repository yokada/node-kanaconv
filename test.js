var kanaconv = require('./build/Release/kanaconv');

//console.log(kanaconv.hello()); // hello, world が出力される

// r: 「全角英字」を「半角」に変換します。
console.log('結果:', kanaconv.convert("０１２３　:ａｂｃ/ＡＢＣ\n", 'r'));

// n: 「全角数字」を「半角」に変換します。
console.log('結果:', kanaconv.convert("０１２３　:ａｂｃ/ＡＢＣ\n", 'n'));

// a: 「全角英数字」を「半角」に変換します。
console.log('結果:', kanaconv.convert("０１２３　:ａｂｃ/ＡＢＣ\n", 'a'));

// s: 「全角スペース」を「半角」に変換します。
console.log('結果:', kanaconv.convert("０１２３　:ａｂｃ/ＡＢＣ\n", 's'));

// k: 「全角カタカナ」を「半角」に変換します。
console.log('結果:', kanaconv.convert("ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ\n", 'k'));
