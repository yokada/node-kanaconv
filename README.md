## 前提

- Python 2.7推奨、Python 3系はサポート対象外
- Xcode command line tool が必要

## 準備

```
$ npm install -g node-gyp
```

```
$ npm install
```

## build

```
$ node-gyp clean
$ node-gyp configure
$ node-gyp build
```

Or:

```
$ node-gyp rebuild
```

## run example

```
$ node test.js
```
