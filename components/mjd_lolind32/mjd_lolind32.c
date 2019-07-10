/*
 * Goto README.md for instructions
 */

// Component header file(s)
#include "mjd.h"
#include "mjd_lolind32.h"

/*
 * Logging
 */
static const char TAG[] = "mjd_lolind32";

/*
 * Data definitions
 */

/**************************************
 * PRIVATE.
 *
 */

/**************************************
 * PUBLIC.
 *
 */
float mjd_lolind32_get_battery_voltage() {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    /**************************************************************************
     * Reuseable variables
     */
    esp_err_t f_retval = ESP_OK;
    float battery_voltage_float = 0;

    // FUNC
    const uint32_t adc_voltage_reference = 1100; // @unit millivolt. Reference Voltage = +-1100mV. This value is not used for measuring the battery voltage because the LOLIN D32 has eFuse "VREF Voltage Reference" BLOCK0.
    const adc_bits_width_t adc_width = ADC_WIDTH_BIT_10;    // BIT_10=0..1023
    const adc1_channel_t adc_channel = ADC1_GPIO35_CHANNEL; // ADC1_GPIO35_CHANNEL ADC1_CHANNEL_7_GPIO_NUM
    const adc_atten_t adc_atten = ADC_ATTEN_DB_11;

    // Configure ADC1
    adc1_config_width(adc_width);
    adc1_config_channel_atten(adc_channel, adc_atten);

    // Characterize ADC1
    esp_adc_cal_characteristics_t adc_cal_characteristics =
                { 0 };
    esp_adc_cal_characterize(ADC_UNIT_1, adc_atten, adc_width, adc_voltage_reference, &adc_cal_characteristics);

    // Logic:
    //   1. ADC read 64 samples (an attempt to reduce noise).
    //   2. Deduct actual battery voltage from the ADC reading.
    //   3. Double the mV value per the circuit design for getting the battery voltage (2x 100K voltage divider on PCB)
    //   4. If the Voltage Regulator is disabled Then apply a correction factor.
    uint32_t voltage_mv = 0;
    uint32_t one_voltage_read = 0;
    const uint32_t NBR_OF_SAMPLES = 64;
    for (int i = 0; i < NBR_OF_SAMPLES; i++) {
        f_retval = esp_adc_cal_get_voltage(adc_channel, &adc_cal_characteristics, &one_voltage_read);
        if (f_retval != ESP_OK) {
            ESP_LOGE(TAG, "%s(). error returned from esp_adc_cal_get_voltage() | err %i (%s)", __FUNCTION__, f_retval,
                    esp_err_to_name(f_retval));
            // GOTO
            goto cleanup;
        }
        voltage_mv += one_voltage_read;
    }
    voltage_mv /= NBR_OF_SAMPLES;
    voltage_mv *= 2;
    ESP_LOGD(TAG, "voltage_mv (*2): %u", voltage_mv);

    battery_voltage_float = voltage_mv / 1000.0;
    ESP_LOGD(TAG, "battery_voltage_float: %f", battery_voltage_float);

    // LABEL
    cleanup: ;

    // RETURN
    return battery_voltage_float;
}

void mjd_lolind32_log_adc_characterisations() {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "REPORT: ADC eFuses availability:");
    ESP_LOGI(TAG,
            "  @doc If both 'eFuse Two Point' and 'eFuse Vref' are not supported then the ADC Calibration logic will use the DEFAULT_VREF Voltage Reference that you provided manually.");

    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        ESP_LOGI(TAG, "  - eFuse Two Point: supported");
    }
    else {
        ESP_LOGI(TAG, "  - eFuse Two Point: NOT supported");
    }

    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        ESP_LOGI(TAG, "  - eFuse Vref: supported");
    }
    else {
        ESP_LOGI(TAG, "  - eFuse Vref: NOT supported");
    }

    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "REPORT: ADC Calibration Characterisation:");
    const uint32_t adc_voltage_reference = 1100; // @unit millivolt. Reference Voltage = +-1100mV. //Use adc2_vref_to_gpio() to obtain a better estimate
    const adc_bits_width_t adc_width = ADC_WIDTH_BIT_10;    // BIT_10=0..1023
    const adc_atten_t adc_atten = ADC_ATTEN_DB_11;

    esp_adc_cal_characteristics_t adc_cal_characteristics =
                { 0 };
    esp_adc_cal_value_t calibration_value_type = esp_adc_cal_characterize(ADC_UNIT_1, adc_atten, adc_width,
            adc_voltage_reference, &adc_cal_characteristics);

    if (calibration_value_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        ESP_LOGI(TAG, "  - ESP_ADC_CAL_VAL_EFUSE_VREF Using eFuse Vref");
    }
    else if (calibration_value_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        ESP_LOGI(TAG, "  - ESP_ADC_CAL_VAL_EFUSE_TP Using Two Point");
    }
    else if (calibration_value_type == ESP_ADC_CAL_VAL_DEFAULT_VREF) {
        ESP_LOGI(TAG, "  - ESP_ADC_CAL_VAL_DEFAULT_VREF Using the manually supplied Default Vref (typically 1100mV)");
    }
    else {
        ESP_LOGW(TAG, "  Warning: unknown calibration value type!");
    }
}

esp_err_t mjd_lolind32_route_vref_to_gpio(int param_gpio_nr) {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    /**************************************************************************
     * Reuseable variables
     */
    esp_err_t f_retval = ESP_OK;

    /**************************************************************************
     * Main
     */
    f_retval = adc2_vref_to_gpio(param_gpio_nr);
    if (f_retval != ESP_OK) {
        ESP_LOGE(TAG, "%s(). ABORT. adc2_vref_to_gpio() GPIO %u() | err %i (%s)", __FUNCTION__, param_gpio_nr, f_retval,
                esp_err_to_name(f_retval));
        // GOTO
        goto cleanup;
    }

    /**************************************************************************
     * LABEL cleanup
     */
    cleanup: ;

    // RETURN
    return f_retval;
}
