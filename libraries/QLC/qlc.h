void qlcDumpDef() {

  uint8_t pcount = ARRAY_SIZE(gPatterns);
  uint8_t addrMin = 0;
  uint8_t width = (255 / pcount);

  Serial.println(" <Channel Name=\"Pattern\">");
  Serial.println("  <Group Byte=\"0\">Gobo</Group>");
  for(int p = 0; p < pcount; p++) {
    Serial.printf("  <Capability Min=\"%u\" Max=\"%u\">%s</Capability>\n", addrMin, (addrMin + width), gPatterns[p].name.c_str());
    addrMin += width + 1;
  }
  Serial.println(" </Channel>");

  Serial.println(" <Channel Name=\"Palette\">");
  Serial.println("  <Group Byte=\"0\">Colour</Group>");
  for(int p = 0; p < ARRAY_SIZE(palettes); p++) {
    CRGB color = ColorFromPalette(palettes[p].palette, 125);
    char hexout[8];
    sprintf(hexout,"#%02x%02x%02x",color.r, color.g, color.b);
    Serial.printf("  <Capability Min=\"%u\" Max=\"%u\" Preset=\"ColorMacro\" Res1=\"%s\">%s</Capability>\n", p, p, hexout, palettes[p].name.c_str());
  }
  Serial.println(" </Channel>");

}
