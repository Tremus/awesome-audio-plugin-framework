### Notes

On macOS, Pugl uses coordinates in the _backing coordinate space_ [read more](https://developer.apple.com/documentation/appkit/nswindow/1419459-backingscalefactor?language=objc). This comes into conflict with CPLUG which doesn't use the backing coordinate space, and NanoVG which uses the non-backing space for drawing.

Any time we send coordinates to Pugl from our plugin, we must multiply the coords by the backing scale factor. Furthermore, when we receive coords from Pugl, we must divide them by the scale factor.

A funny quirk of NanoVG is that all drawing is done in a non-backing coordinate space, but any time you create an image or framebuffer and resize your viewport you need to use coordinates in the backing space. `nvgBeginFrame` is particularly strange, in that it expects w/h in non-backing space + your `devicePixelRatio` (backingScaleFactor). Unfornately this doesn't translate to windows very well which seemingly doesn't mess about with a backingScaleFactor, so you usually get 1.0 when you ask for it (macOS usually returns 2.0). The shaders in NanoVG alias a lot when you use 1:1 scaling, so I've set things up to **always use 2:1 scaling**.

Got all that? Good.

This template is all set up to _just work_, but just keep this in mind when your start changing how your app responds to resizing and mouse events.