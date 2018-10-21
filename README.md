## WIP - MouseWarping guidance overlay

### What

See the [original
issue](https://github.com/keyboardio/Kaleidoscope-MouseKeys/issues/30) for
motivation and *screenshots* and [the followup
issue](https://github.com/keyboardio/Kaleidoscope/issues/372) for ongoing
discussion.

### How

Currently we rely on libaosd which in turn depends on cairo+pango to render the overlay.

Running:

    make

will call cmake and if everything works out you can launch mousewarp with:

    build/mousewarp

Alternatively, if you have nix (nix-shell) on your system simply clone the repository and:

    make run
