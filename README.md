magicbot
========

# Magicbot
This horrible monstrosity started out as one of the ugliest bash scripts I've ever written.  
Credit to cmurphy for all the nice parts in magicbot.sh, which are almost entirely borrowed from https://github.com/cmurphy/d2lminusminus.

Since then it's metastasized into a project to rewrite the whole thing in C, with a local JSON database pulled from mtgjson.com.
Multithreading is now in the picture, with capabilities for multiple simultaneous lookups and a rate-limited output queue.
God help me.
