#ifndef PALETTES_DEFINED
#define PALETTES_DEFINED

Colors::RGB monster[] = { // plasma!!
  Colors::RGB(0, 184, 255), //bright cyan
  Colors::RGB(0, 184, 255), //bright cyan
  Colors::RGB(115, 255, 0), //lime green
  Colors::RGB(0, 184, 255), //bright cyan
  Colors::RGB(115, 255, 0), //lime green
  Colors::RGB(163, 0, 255) //deep lavendar
};
Palette monsterPalette = Palette(monster, 6); //like it

Colors::RGB sunset[] = {
  Colors::RGB(255, 0, 0), //red
  Colors::RGB(255, 0, 180), //pink
  Colors::RGB(255, 121, 0), //burn orange
  Colors::RGB(255, 198, 0) //golden yellow
  };
Palette sunsetPalette = Palette(sunset, 4); //love it

Colors::RGB liz[] = {
  Colors::RGB(202, 0, 255), //phonecasepink
  Colors::RGB(0, 158, 255), //dark cyan
  Colors::RGB(35, 18, 241), //dark blue
  Colors::RGB(108, 0, 255) //purple
};
Palette lizPalette = Palette(liz, 4); //maybe drop this one

Colors::RGB flower[]= {
  Colors::RGB(246, 255, 0), //yellow
  Colors::RGB(255, 0, 102), //pink
  Colors::RGB(255, 54, 0), //red
  Colors::RGB(78, 255, 0) //green
};
Palette flowerPalette = Palette(flower, 4);

Colors::RGB macaw[]= {
  Colors::RGB(255, 0, 0), //red
  Colors::RGB(255, 0, 0), //red
  Colors::RGB(255, 201, 0), //gold
  Colors::RGB(67, 189, 9), //dark green
  Colors::RGB(0, 160, 219), //cyan
  Colors::RGB(0, 160, 219), //cyan
  Colors::RGB(9, 90, 255) //blueish
};
Palette macawPalette = Palette(macaw, 7);


Colors::RGB ocean[]= {
  Colors::RGB(29, 75, 225), //dark blue
  Colors::RGB(33, 174, 225), //surf blue
  Colors::RGB(33, 211, 255), //cyan
  Colors::RGB(30, 255, 205) //sea green
};
Palette oceanPalette = Palette(ocean, 4);

Colors::RGB angelfish[]= {
  Colors::RGB(255, 126, 0), //orange
  Colors::RGB(30, 62, 255), //purplish blue
  Colors::RGB(48, 304, 255), //cyan
  Colors::RGB(184, 255, 48), //greenish
  Colors::RGB(155, 255, 48), //greeenish yellow
  Colors::RGB(255, 67, 48) //redish
};
Palette angelfishPalette = Palette(angelfish, 6);

Colors::RGB flame[]= {
  Colors::RGB(255, 60, 0),
  Colors::RGB(255, 72, 0), //red
  Colors::RGB(255, 102, 0), //dark orange
  Colors::RGB(255, 228, 0) //yellow
};
Palette flamePalette = Palette(flame, 4);

Colors::RGB machu[]= {
  Colors::RGB(255, 156, 0), //clay
  Colors::RGB(120, 255, 67), //green
  Colors::RGB(0, 141, 112), //forest green
  Colors::RGB(0, 129, 203), //blueish
  Colors::RGB(0, 192, 255) //sky blue
};
Palette machuPalette = Palette(machu, 5);

Colors::RGB snek[]= {
  Colors::RGB(0, 162, 255), //cyan
  Colors::RGB(0, 0, 255), //dark blue
  Colors::RGB(234, 255, 0), //yellow
  Colors::RGB(255, 120, 0), //orangeish
};
Palette snekPalette = Palette(snek, 4);

Colors::RGB starling[]= {
  Colors::RGB(108, 0, 255), //dark purple
  Colors::RGB(180, 36, 255), //lavendar
  Colors::RGB(0, 36, 255), //darkish blue
  Colors::RGB(36, 211, 255), //lighter blue
  Colors::RGB(0, 255, 198), //foamish green
};
Palette starlingPalette = Palette(starling, 5);

Colors::RGB rasta[]= {
  Colors::RGB(255, 0, 0), //red
  Colors::RGB(255, 198, 0), //yellow
  Colors::RGB(47, 192, 0), //green
};
Palette rastaPalette = Palette(rasta, 3);

Colors::RGB primary[]= {
  Colors::RGB(255, 0, 210), //pink
  Colors::RGB(255, 0, 24), //red
  Colors::RGB(0, 108, 255), //blue
  Colors::RGB(177, 0, 255) // lavendar
};
Palette primaryPalette = Palette(primary, 4);

Colors::RGB party[]= {
  Colors::RGB(255, 77, 0), //redish
  Colors::RGB(255, 98, 0), //orangish
  Colors::RGB(255, 156, 0), //goldish
  Colors::RGB(156, 255, 0), // greenish
  Colors::RGB(0, 144, 255) //blue
};
Palette partyPalette = Palette(party, 5);

Colors::RGB psychadelic[]= {
  Colors::RGB(255, 60, 0), //redish
  Colors::RGB(255, 156, 0), //orangish
  Colors::RGB(255, 216, 0), //yellow
  Colors::RGB(150, 255, 0), //light green
  Colors::RGB(0, 66, 255) //blue
};
Palette psychadelicPalette = Palette(psychadelic, 5);

Palette palettes[] = { psychadelicPalette, partyPalette, primaryPalette, rastaPalette, starlingPalette, snekPalette,  machuPalette, flamePalette, monsterPalette, sunsetPalette, lizPalette, flowerPalette, macawPalette, oceanPalette, angelfishPalette };

#endif
