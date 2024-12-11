<h1 align="center">Minilab3</h1>

<p align="center">
A pad-multiplexer for <a href="https://www.arturia.com/products/hybrid-synths/minilab-3/overview">Aturia's Minilab3</a> and <a href="https://vcvrack.com/">VCV Rack</a>.
</p>

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/main/docs/public/controller.png?raw=true" alt="Minilab3"/>
</p>

<p align="center">
The VCV Rack module.
</p>

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/main/docs/public/hero.png?raw=true" alt="Minilab3 module"/>
</p>


# Multiplexer?

Imagine you had *hundreds* of MIDI knobs and mod wheels.

For each pad on the Minilab3, pressing it will get you a whole new set of the non-pad surfaces.

As an example, say I just twist the Minilab3's first knob. We get red activity. But if I hold down the first pad, an independently tracked blue knob value comes out of the expander module:

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/main/docs/public/scope.gif?raw=true" alt="Minilab3 module"/>
</p>

The Aturia Minilab3 has 8 pads, so that's already 64 logical knobs.

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/main/docs/public/8-pads.png?raw=true" alt="Minilab3 module"/>
</p>

*Buuuut*, it has *two banks* of pads making for 16 pads, or **128 logical knobs**!

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/main/docs/public/16-pads.png?raw=true" alt="Minilab3 module"/>
</p>

And *technically*, there's 15 available channels so you *could* take it to *1,920 knobs*...


<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/main/docs/public/15-channels.png?raw=true" alt="Minilab3 module"/>
</p>

...but your computer will give up first lol.