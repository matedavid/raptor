Raptor
---
Just another browser

Using for GUI:
* gtk3
* gtkmm3

CSS Selectors: https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Selectors

TODO:
- [ ] Fix CSS parser does not accept /* */ comments (error on '/') character
- [ ] Fix CSS parser error where it does not recognize '.' character in values that are floats (example: `padding: 0.5em;`)
- [ ] Implement '#' character in CSS values used to specify colors