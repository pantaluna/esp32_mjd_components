/*
 * Goto the README.md for instructions
 *
 */

/*
 * Includes: system, own
 */
#include "mjd.h"
#include "mjd_lorabee.h"
#include "mjd_lorap2p.h"

/*
 * Logging
 */
static const char TAG[] = "mjd_lorap2p";

/*
 * MUTEX
 * @doc For future use.
 */
static SemaphoreHandle_t _lorap2p_service_semaphore = NULL;
#define MJD_LORAP2P_SERVICE_LOCK()     xSemaphoreTake(_lorap2p_service_semaphore, portMAX_DELAY)
#define MJD_LORAP2P_SERVICE_UNLOCK()   xSemaphoreGive(_lorap2p_service_semaphore)

/*
 * Channel
 *     #1 868.90 Mhz 868900000 [SF7 BW125 CR4/8]
 *     #2 869.10 Mhz 869100000 [SF7 BW125 CR4/8]
 *     #3 869.50 Mhz 869500000 [SF7 BW125 CR4/8]
 *     #4 869.80 Mhz 869800000 [SF7 BW125 CR4/8]
 */
static mjd_lorap2p_radio_channel_config_t _mjd_lorap2p_radio_channel_configs[4] =
            {
                        {
                                868900000,
                                MJD_LORABEE_SPREADING_FACTOR_SF7,
                                MJD_LORABEE_BANDWIDTH_125KHZ,
                                MJD_LORABEE_CODING_RATE_4_8 },
                        {
                                869100000,
                                MJD_LORABEE_SPREADING_FACTOR_SF7,
                                MJD_LORABEE_BANDWIDTH_125KHZ,
                                MJD_LORABEE_CODING_RATE_4_8 },
                        {
                                869500000,
                                MJD_LORABEE_SPREADING_FACTOR_SF7,
                                MJD_LORABEE_BANDWIDTH_125KHZ,
                                MJD_LORABEE_CODING_RATE_4_8 },
                        {
                                869800000,
                                MJD_LORABEE_SPREADING_FACTOR_SF7,
                                MJD_LORABEE_BANDWIDTH_125KHZ,
                                MJD_LORABEE_CODING_RATE_4_8 }
            };

/**************************************
 * PUBLIC.
 *
 */
/*
 * @brief helper logging
 *
 */
esp_err_t mjd_lorap2p_log_config(mjd_lorap2p_config_t* param_ptr_config) {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;

    ESP_LOGI(TAG, "Log mjd_lorap2p_config_t* param_ptr_config:");

    ESP_LOGI(TAG, "  %32s = %u", "_is_init", param_ptr_config->_is_init);

    ESP_LOGI(TAG, "  %32s = %i", "uart_port_t uart_port_num", param_ptr_config->uart_port_num);
    ESP_LOGI(TAG, "  %32s = %i", "gpio_num_t uart_tx_gpio_num", param_ptr_config->uart_tx_gpio_num);
    ESP_LOGI(TAG, "  %32s = %i", "gpio_num_t uart_rx_gpio_num", param_ptr_config->uart_rx_gpio_num);
    ESP_LOGI(TAG, "  %32s = %i", "gpio_num_t reset_gpio_num", param_ptr_config->reset_gpio_num);

    ESP_LOGI(TAG, "  %32s = "MJDMACFMT, "radio_device_address", MJDMAC2STR(param_ptr_config->radio_device_address));

    ESP_LOGI(TAG, "  %32s = %i", "radio_power", param_ptr_config->radio_power);
    ESP_LOGI(TAG, "  %32s = %i", "radio_channel", param_ptr_config->radio_channel);
    ESP_LOGI(TAG, "  %32s = %u millisec", "radio_watchdog_timeout", param_ptr_config->radio_watchdog_timeout);
    ESP_LOGI(TAG, "  %32s = %u", "tx_x_times", param_ptr_config->tx_x_times);
    ESP_LOGI(TAG, "  %32s = %u", "_nbr_of_errors", param_ptr_config->_nbr_of_errors);

    // loraBEE instance:
    mjd_lorabee_log_config(&param_ptr_config->lorabee_config);

    // LABEL
    /////cleanup: ;

    return f_retval;
}

esp_err_t mjd_lorap2p_log_data_frame_input(mjd_lorap2p_data_frame_input_t *param_ptr_data) {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;

    ESP_LOGI(TAG, "Log mjd_lorap2p_data_frame_input_t* param_ptr_data:");

    ESP_LOGI(TAG, "  %32s = "MJDMACFMT, "destination_address", MJDMAC2STR(param_ptr_data->destination_address));
    ESP_LOGI(TAG, "  %32s = %u", "len_payload", param_ptr_data->len_payload);
    ESP_LOGI(TAG, "  %32s = %s", "payload", param_ptr_data->payload);
    ESP_LOG_BUFFER_HEXDUMP(TAG, param_ptr_data->payload, param_ptr_data->len_payload, ESP_LOG_INFO);

    // LABEL
    /////cleanup: ;

    return f_retval;
}

esp_err_t mjd_lorap2p_log_data_frame(mjd_lorap2p_data_frame_t *param_ptr_data) {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;

    ESP_LOGI(TAG, "Log mjd_lorap2p_data_frame_t* param_ptr_data:");

    ESP_LOGI(TAG, "  %32s = %c%c%c", "_prefix", param_ptr_data->_prefix[0], param_ptr_data->_prefix[1], param_ptr_data->_prefix[2]);
    ESP_LOGI(TAG, "  %32s = %u", "_frame_type", param_ptr_data->_frame_type);
    ESP_LOGI(TAG, "  %32s = "MJDMACFMT, "source_address", MJDMAC2STR(param_ptr_data->source_address));
    ESP_LOGI(TAG, "  %32s = %u", "seq_nr", param_ptr_data->seq_nr);
    ESP_LOGI(TAG, "  %32s = %u", "is_retry", param_ptr_data->is_retry);
    ESP_LOGI(TAG, "  %32s = "MJDMACFMT, "destination_address", MJDMAC2STR(param_ptr_data->destination_address));
    ESP_LOGI(TAG, "  %32s = %u", "len_payload", param_ptr_data->len_payload);
    ESP_LOGI(TAG, "  %32s = %s", "payload", param_ptr_data->payload);
    ESP_LOG_BUFFER_HEXDUMP(TAG, param_ptr_data->payload, param_ptr_data->len_payload, ESP_LOG_INFO);
    ESP_LOGI(TAG, "");

    // LABEL
    /////cleanup: ;

    return f_retval;
}

/*
 * Init ^& Deinit
 */
esp_err_t mjd_lorap2p_init(mjd_lorap2p_config_t* param_ptr_config) {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;

    // TODO Check all params

    if (param_ptr_config->_is_init == true) {
        f_retval = ESP_ERR_INVALID_STATE;
        ESP_LOGE(TAG, "%s(). The component has already been init'd | err %i (%s)", __FUNCTION__, f_retval,
                esp_err_to_name(f_retval));
        // GOTO
        goto cleanup;
    }

    // MUTEX (for future use; TODO Handle conflicts accessing data from multiple tasks)
    if (!_lorap2p_service_semaphore) {
        _lorap2p_service_semaphore = xSemaphoreCreateMutex();
        if (!_lorap2p_service_semaphore) {
            f_retval = ESP_FAIL;
            ESP_LOGE(TAG, "%s(). xSemaphoreCreateMutex() err %i (%s)", __FUNCTION__, f_retval, esp_err_to_name(f_retval));
            // GOTO
            goto cleanup;
        }
    }

    // LORAP2P Data Structure
    esp_efuse_mac_get_default(param_ptr_config->radio_device_address);

    // LORABEE
    param_ptr_config->lorabee_config.uart_port_num = param_ptr_config->uart_port_num;
    param_ptr_config->lorabee_config.uart_tx_gpio_num = param_ptr_config->uart_tx_gpio_num;
    param_ptr_config->lorabee_config.uart_rx_gpio_num = param_ptr_config->uart_rx_gpio_num;
    param_ptr_config->lorabee_config.reset_gpio_num = param_ptr_config->reset_gpio_num;

    param_ptr_config->lorabee_config.radio_power = param_ptr_config->radio_power;
    param_ptr_config->lorabee_config.radio_mode = MJD_LORABEE_MODE_LORA;
    param_ptr_config->lorabee_config.radio_frequency =
            _mjd_lorap2p_radio_channel_configs[param_ptr_config->radio_channel].frequency;
    param_ptr_config->lorabee_config.radio_spreading_factor =
            _mjd_lorap2p_radio_channel_configs[param_ptr_config->radio_channel].spreading_factor;
    param_ptr_config->lorabee_config.radio_bandwidth =
            _mjd_lorap2p_radio_channel_configs[param_ptr_config->radio_channel].bandwidth;
    param_ptr_config->lorabee_config.radio_coding_rate =
            _mjd_lorap2p_radio_channel_configs[param_ptr_config->radio_channel].coding_rate;
    param_ptr_config->lorabee_config.radio_watchdog_timeout = param_ptr_config->radio_watchdog_timeout;

    f_retval = mjd_lorabee_init(&param_ptr_config->lorabee_config);
    if (f_retval != ESP_OK) {
        ESP_LOGE(TAG, "mjd_lorabee_init() err %i (%s)", f_retval, esp_err_to_name(f_retval));
        // GOTO
        goto cleanup;
    }

    // Mark init-yes
    param_ptr_config->_is_init = true;

    // LABEL
    cleanup: ;

    return f_retval;
}

esp_err_t mjd_lorap2p_deinit(mjd_lorap2p_config_t* param_ptr_config) {
    // @important Do not erase my static data structure _lorap2p_data so the app can still retrieve the last readings.

    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;

    // LORABEE
    f_retval = mjd_lorabee_deinit(&param_ptr_config->lorabee_config);
    if (f_retval != ESP_OK) {
        ESP_LOGE(TAG, "mjd_lorabee_deinit() err %i (%s)", f_retval, esp_err_to_name(f_retval));
        // GOTO
        goto cleanup;
    }

    // Mark init'd false
    param_ptr_config->_is_init = false;

    // LABEL
    cleanup: ;

    return f_retval;
}

/*
 * @brief RADIO TX
 *
 * @rule EU863-870 Maximum payload size ASCII 230 => HEXSTR 460
 * @dep mjd_lorabee_config_t->max_nbr_of_radio_tx
 * TODO More research FIX TOO LONG DELAY 5SEC AFTER SENDING PACKET and waiting for response (duration varies with quality of Lora network/environment!)
 *
 * @techdoc
 *  Response after entering the command:
 *      # ok            – if parameter is valid and the transceiver is configured in Transmit mode ==> OK, CONTINUE
 *      # invalid_param – if parameter is not valid ==> ERROR
 *      # busy          – if the transceiver is currently busy ==> RETRY (TODO)
 *  Response after the effective transmission:
 *      # radio_tx_ok – if transmission was successful ==> OK
 *      # radio_err   – if transmission was unsuccessful (interrupted by radio Watchdog Timer time-out)  ==> RETRY
 *
 * @doc Also handles `radio tx`: response#1 'busy' as an error
 *
 */
esp_err_t mjd_lorap2p_tx(mjd_lorap2p_config_t* param_ptr_config,
                         const mjd_lorap2p_data_frame_input_t *param_ptr_data_frame_input) {
    ESP_LOGD(TAG, "%s()", __FUNCTION__);

    esp_err_t f_retval = ESP_OK;

    static uint8_t _frame_seq_nr = 0; // @important static
    mjd_lorap2p_data_frame_t data_frame = MJD_LORAP2P_DATA_FRAME_DEFAULT();

    /*
     * Check input params
     */
    if (param_ptr_data_frame_input->len_payload > MJD_LORAP2P_TX_PAYLOAD_MAX_BYTES) {
        f_retval = ESP_FAIL;
        ESP_LOGE(TAG, "ABORT %s(). P2P Payload length %u exceeds maximum (%u bytes) | err %i (%s)", __FUNCTION__,
                param_ptr_data_frame_input->len_payload,
                MJD_LORAP2P_TX_PAYLOAD_MAX_BYTES, f_retval, esp_err_to_name(f_retval));
        // GOTO
        goto cleanup;
    }

    /*
     * Transfer input params param_ptr_data_frame_input => data_frame
     * & Fill derived & calculated values
     */
    memcpy(data_frame.source_address, param_ptr_config->radio_device_address, sizeof(data_frame.source_address));
    data_frame.seq_nr = ++_frame_seq_nr;
    data_frame.is_retry = 0; // First tx is not a retry-frame
    memcpy(data_frame.destination_address, param_ptr_data_frame_input->destination_address,
            sizeof(data_frame.destination_address));
    data_frame.len_payload = param_ptr_data_frame_input->len_payload;
    data_frame.payload = param_ptr_data_frame_input->payload;

    mjd_lorap2p_log_data_frame(&data_frame);

    uint8_t* ptr_payload_data_frame;
    uint8_t payload_lorabee_[1024] = { 0 };
    size_t pos;

    /*
     * TX x times
     */
    for (uint8_t iter = 0; iter < param_ptr_config->tx_x_times; iter++) {
        ESP_LOGI(TAG, "%s(). TX LOOP iter #%u of %u", __FUNCTION__, 1+iter, param_ptr_config->tx_x_times);

        /*
         * Serialize for transmission
         * TODO Use protobuf??? (I need to RX the data it in Node.js+USBUART so serdata must be platform independent!)
         */
        pos = 0;
        payload_lorabee_[pos++] = data_frame._prefix[0];
        payload_lorabee_[pos++] = data_frame._prefix[1];
        payload_lorabee_[pos++] = data_frame._prefix[2];
        payload_lorabee_[pos++] = data_frame._frame_type;
        payload_lorabee_[pos++] = data_frame.source_address[0];
        payload_lorabee_[pos++] = data_frame.source_address[1];
        payload_lorabee_[pos++] = data_frame.source_address[2];
        payload_lorabee_[pos++] = data_frame.source_address[3];
        payload_lorabee_[pos++] = data_frame.source_address[4];
        payload_lorabee_[pos++] = data_frame.source_address[5];
        payload_lorabee_[pos++] = data_frame.seq_nr;
        payload_lorabee_[pos++] = data_frame.is_retry;
        payload_lorabee_[pos++] = data_frame.destination_address[0];
        payload_lorabee_[pos++] = data_frame.destination_address[1];
        payload_lorabee_[pos++] = data_frame.destination_address[2];
        payload_lorabee_[pos++] = data_frame.destination_address[3];
        payload_lorabee_[pos++] = data_frame.destination_address[4];
        payload_lorabee_[pos++] = data_frame.destination_address[5];
        payload_lorabee_[pos++] = data_frame.len_payload;
        ptr_payload_data_frame = data_frame.payload;
        for (int iter=0; iter < data_frame.len_payload; iter++) {
            payload_lorabee_[pos++] = *(ptr_payload_data_frame);
            ptr_payload_data_frame++;
        }
        size_t len_payload_lorabee = pos; // @important Can be bigger than 255 so cannot use uint8_t

        ESP_LOGD(TAG, "");
        ESP_LOGD(TAG, "TX raw payload");
        ESP_LOGD(TAG, "  %32s = %zu", "len_payload_lorabee", len_payload_lorabee);
        ESP_LOG_BUFFER_HEXDUMP(TAG, payload_lorabee_, len_payload_lorabee, ESP_LOG_DEBUG);
        ESP_LOGD(TAG, "");

        /*
         * LORABEE logic
         */
        f_retval = mjd_lorabee_radio_tx(&param_ptr_config->lorabee_config, payload_lorabee_, len_payload_lorabee);
        if (f_retval != ESP_OK) {
            ++param_ptr_config->_nbr_of_errors;
            ESP_LOGE(TAG, "ABORT %s(). mjd_lorabee_radio_tx() failed | err %i (%s)", __FUNCTION__, f_retval,
                    esp_err_to_name(f_retval));
            goto cleanup;
        }

        // Mark frame#2..n as retries! SO they can be de-duplicated at the Edge Forwarder Service (Node.js).
        data_frame.is_retry = 1;
    }

    // LABEL
    cleanup: ;

    return f_retval;
}
