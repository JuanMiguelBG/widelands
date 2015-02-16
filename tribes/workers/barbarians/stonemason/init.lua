dirname = path.dirname(__file__)

animations = {
   idle = {
      pictures = { dirname .. "idle_\\d+.png" },
      hotspot = { 8, 21 },
      fps = 10
   },
   hacking = {
      pictures = { dirname .. "hacking_\\d+.png" },
      hotspot = { 8, 21 },
      fps = 10
   }
}
add_worker_animations(animations, "walk", dirname, "walk", {9, 24}, 10)
add_worker_animations(animations, "walkload", dirname, "walkload", {7, 22}, 10)


tribes:new_worker_type {
   name = "barbarians_stonemason",
   -- TRANSLATORS: This is a worker name used in lists of workers
   descname = _"Stonemason",
   icon = dirname .. "menu.png",
   vision_range = 2,

   buildcost = {
		barbarians_carrier = 1,
		pick = 1
	},

	programs = {
		cut_granite = {
			"findobject attrib:granite radius:6",
			"walk object",
			"playFX ../../../sound/stonecutting/stonecutter 192",
			"animation hacking 10000",
			"object shrink",
			"createware granite",
			"return"
		}
	},

	-- TRANSLATORS: Helptext for a worker: Stonemason
   helptext = _"Cuts raw pieces of granite out of rocks in the vicinity.",
   animations = animations,
}
