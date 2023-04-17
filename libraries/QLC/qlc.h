void qlcDumpDef() {

  int pcount = ARRAY_SIZE(gPatterns);
  int addrMin = 0;
  uint8_t width = (255 / pcount);

  for(int p = 0; p < pcount; p++) {
    Serial.printf("   <Capability Min=\"%u\" Max=\"%u\">%s</Capability>\n", addrMin, (addrMin + width), gPatterns[p].name.c_str());
    addrMin += width + 1;
  }
}
