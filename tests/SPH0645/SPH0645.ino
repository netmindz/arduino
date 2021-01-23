#include <driver/i2s.h>

#define SAMPLE_RATE 32000
#define BLOCK_SIZE 512

const i2s_port_t I2S_PORT = I2S_NUM_0;

#define I2S_WS 15   // LRCL
#define I2S_SD 32   // DOUT
#define I2S_SCK 14  // BCLK

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  esp_err_t err;

  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    // .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = BLOCK_SIZE,
    .use_apll = true,
  };
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  err = i2s_driver_install(I2S_PORT, &i2s_config, 0, nullptr);
  if (err != ESP_OK) {
    Serial.printf("Failed installing driver: %d\n", err);
  }

  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("Failed setting pins: %d\n", err);
  }


  // REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));
  // REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);

  i2s_set_sample_rates(I2S_PORT, SAMPLE_RATE); //set sample rates
}


void loop() {

  size_t bytes_read;
  int32_t samples[BLOCK_SIZE];
  int32_t read_samples = 0;
  // unsigned long before = micros();
  // unsigned long after = 0;
  unsigned long before = micros();
  while (read_samples < BLOCK_SIZE) {
    esp_err_t err = i2s_read(I2S_PORT, (void *)samples, BLOCK_SIZE * 4, &bytes_read, portMAX_DELAY);
    if (err != ESP_OK) {
      Serial.printf("Error reading bytes: %d\n", err);
    }
    if (bytes_read <= 0 || bytes_read % 4 != 0) {
      Serial.printf("Invalid amount of bytes read: %d\n", bytes_read);
    }
    else
    {
      for (int i = 0; i < bytes_read / 4; i++) {
        if (read_samples == BLOCK_SIZE)
          break;
        // vReal[read_samples] = (double)(abs(samples[i] >> 14));
        // vReal[read_samples]
        double value = (double)(samples[i] >> 14);
        Serial.print("0 ");
        Serial.println(value);
        // Serial.printf( "%.2f ",(double)(samples[i] >> 14));
        // vImag[read_samples] = 0;
        // x1[read_samples] = abs(samples[i] >> 14);
        // x2[read_samples] = 0.0f;
        read_samples++;
      }
      // Serial.printf("\n");
    }
  }
}
