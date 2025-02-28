# display

Simple TUI library

## Description

This project is my latest attempt to create a modular end extensible TUI
library that takes care of the positioning of the elements on the screen and
their nesting.

As of know it has all of the necessary features for me to start building some
staff with it, and figure out the rest as I go.

## Dependencies

* CMake 3.14 or latter
* Compiler with C++20 support (tested on clang version 19.1.7)
* [`Stamen 1.3.0`](https://github.com/DimitrijeDobrota/stamen) (for running examples)


## Building and installing

See the [`BUILDING`](BUILDING.md) document.


## Usage

> Please reference example folder for relevant usage examples.

The library provides the following classes:
* `Element`: Base class that takes care of the absolute positioning
* `Display`: Main entry point to the terminal screen
* `Layout`: Container for one element
* `LayoutMulti`: Container for multiple elements
* `LayoutRigid`: Divide a screen into custom rectangles
* `Window`: Bass class for drawing to the screen
* `WindowPivot`: Window that can be placed in the corners or center of the layout


## Version History

* 0.2
    * Proof of concept class hierarchy
    * Chained rendering and input propagation
    * Responsive redrawing

* 0.1
    * Initial development phase


## Contributing

See the [`CONTRIBUTING`](CONTRIBUTING.md) document.


## License

This project is licensed under the MIT License - see the [`LICENSE`](LICENSE.md) file for details

