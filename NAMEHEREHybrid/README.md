NAMEHEREHybrid API Reference
---
- []()

Usage
---
In order to use this library, include the following file in your html
```html
<script src="namehere.js" type="text/javascript"></script>
```

Then, you can reference the modules, for example Game, in the following way
```js
NAMEHERE.Hybrid.Game.startWithPlaceID("XXX", myCallback);
```

Each module also has a "supports" function, that executes asynchronously and checks if the queried function is supported natively in the current platform.
```js
	NAMEHERE.Hybrid.Game.supports("followUser", function(isSupported) {
		if(isSupported) {
			NAMEHERE.Hybrid.Game.followUser("XXXXX", myCallback);
		} else {
			// Web implementation
		}
	});
```


How to generate the compiled NAMEHEREHybrid.js file
---
"namehere.js" is compiled and optimized with [Google Closure Compiler](https://github.com/google/closure-compiler)

In order to run this tool execute:
```sh
$ ./build_js.sh
```

How to generate this documentation
---
```sh
$ cd docs
$ ./build_doc.sh
```
