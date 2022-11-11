# The Rust Effect
#### Version 0.5 Intentions

I've spent a little over a month experimenting with Rust as a systems language. There are definitely some great things happening there but I'm not about to abandon C yet. Still, some of the practices make enough sense to affect this library and version 0.5 will see some big changes to the API.

## Vector Naming

C++ implements its dynamic array as a `vector`. In this library I just called it a `cgs_array`. Rust also uses the "vector" terminology so I'm going to change the name of my arrays to "vector". This was a narrow decision initially and now I think its just a good idea to stick with a common name for similar functionality.

## Results and Options

Halfway through implementing my library I started to get a strong feel for return values needing to be indicators of success or failure. As this pattern revealed itself to me I caught myself altering the idiom a few times. I don't doubt there are a few backwards signatures in the library by now. I want to search and destroy all of these variances.

This is similar to the "Result" and "Option" patterns in Rust where on success a value is returned, on failure a falsy value is. In C I've been achieving this with `void*`'s that returned NULL on error and some valid pointer on success. However the value was usually placed as an out-param. Sometimes this out-param was the first parameter, ala the standard library, sometimes the last, similar to other libraries. This needs to be made more consistent.

In version 0.5 a function that can fail will have a `void*` return value to indicate success or failure. Any values produced in the successful completion will be out-params. This out-param will be the last parameter provided to the function.

In version 0.5 a function that cannot fail should return a value unless it doesn't make sense to (such as sort functions).

In version 0.5 effort will be made to codify which functions may or may not fail according to their names (in Rust, `_new()` functions should never fail). Effort will also be made to minimize the number of fail-able functions. An example of this is that strings and vectors should be initiated with a safe `_new()` function without allocation. Initial allocation occurs the first time an attempt to `push` into the container is made.

## Still Immutable By Default

Hey! This one doesn't need to be stated because it was always a project goal! How about that?!
