void qlcDumpDef(PatternAndNameList patterns, PaletteAndNameList palettes) {

  int pcount = ARRAY_SIZE(patterns);
  int addrMin = 0;
  uint8_t width = (255 / pcount);

  Serial.println(" <Channel Name=\"Pattern\">");
  Serial.println("  <Group Byte=\"0\">Gobo</Group>");
  for(int p = 0; p < pcount; p++) {
    Serial.printf("   <Capability Min=\"%u\" Max=\"%u\">%s</Capability>\n", addrMin, (addrMin + width), patterns[p].name.c_str());
    addrMin += width + 1;
  }
  Serial.println(" </Channel>");

  Serial.println(" <Channel Name=\"Palette\">");
  Serial.println("  <Group Byte=\"0\">Colour</Group>");
  for(int p = 0; p < ARRAY_SIZE(palettes); p++) {
    Serial.printf("   <Capability Min=\"%u\" Max=\"%u\">%s</Capability>\n", addrMin, (addrMin + width), palettes[p].name.c_str());
    addrMin += width + 1;
  }
  Serial.println(" </Channel>");

}
