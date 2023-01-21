<h1 align="center">Minilab3</h1>

<p align="center">
A pad-multiplexer for <a href="https://www.arturia.com/products/hybrid-synths/minilab-3/overview">Aturia's Minilab3</a> and <a href="https://vcvrack.com/">VCV Rack</a>.
</p>

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/master/minilab.png?raw=true" alt="Minilab3"/>
</p>

<p align="center">
The VCV Rack module.
</p>

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/master/module.png?raw=true" alt="Minilab3 module"/>
</p>


# Multiplexer?

Imagine you had *hundreds* of MIDI knobs and mod wheels.

For each pad on the Minilab3, pressing it will get you a whole new set of the non-pad surfaces.

As example, say if I just twist the Minilab3's first knob we get blue activity:

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/master/main-knob.png?raw=true" alt="Minilab3 module"/>
</p>

But if I hold down the first pad, an independently tracked knob value comes out of the expander module:

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/master/pad-knob.png?raw=true" alt="Minilab3 module"/>
</p>

The Aturia Minilab3 has 8 pads, so that's already 64 logical knobs.

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/master/8-knobs.png?raw=true" alt="Minilab3 module"/>
</p>

*Buuuut*, it has *two banks* of pad making for 16 pads, or 128 logical knobs!

<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/master/16-knobs.png?raw=true" alt="Minilab3 module"/>
</p>

And *technically*, there's 15 available channels so you *could* take it to *1,920 knobs*...


<p align="center">
  <img src="https://github.com/dustinlacewell/vcv-minilab3/blob/master/15-channels.png?raw=true" alt="Minilab3 module"/>
</p>

...but your computer will probably leave you.
