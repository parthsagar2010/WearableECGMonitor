
#include "defines.h"
#include "MaxAFE.h"
#include "EcgUart.h"
#include "Battery.h"

#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/services/BatteryService.h"
#include "ble/services/HeartRateService.h"
#include "MaxClock.h"
#include "BLE_Interface.h"

const char ECG_UUID[] = "485f4145-52b9-4644-af1f-7a6b9322490f";
const char ECG_SERVICE_UUID[] = "51311102-030e-485f-b122-f8f381aa84ed";
  BLE &bleX = BLE::Instance();
  ECG_Monitor AFEBLE(bleX, event_queue);
 

int main()
{
	MaxAFE_Clock_Init();
	MaxAFE_t::EcgAfeInit();						//does not work with ble thread related??

	bleX.onEventsToProcess(schedule_ble_events);
	AFEBLE.start();
		while(1){__WFE ();};
}



///* mbed Microcontroller Library
// * Copyright (c) 2017 ARM Limited
// *
// * Licensed under the Apache License, Version 2.0 (the "License");
// * you may not use this file except in compliance with the License.
// * You may obtain a copy of the License at
// *
// *     http://www.apache.org/licenses/LICENSE-2.0
// *
// * Unless required by applicable law or agreed to in writing, software
// * distributed under the License is distributed on an "AS IS" BASIS,
// * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// * See the License for the specific language governing permissions and
// * limitations under the License.
// */

//#include <stdio.h>

//#include "platform/Callback.h"
//#include "events/EventQueue.h"
//#include "platform/NonCopyable.h"

//#include "ble/BLE.h"
//#include "ble/Gap.h"
//#include "ble/GattClient.h"
//#include "ble/GapAdvertisingParams.h"
//#include "ble/GapAdvertisingData.h"
//#include "ble/GattServer.h"
//#include "BLEProcess.h"

//using mbed::callback;
//const char ECG_UUID[] = "485f4145-52b9-4644-af1f-7a6b9322490f";
//const char ECG_SERVICE_UUID[] = "51311102-030e-485f-b122-f8f381aa84ed";
///**
// * A Clock service that demonstrate the GattServer features.
// *
// * The clock service host three characteristics that model the current hour,
// * minute and second of the clock. The value of the second characteristic is
// * incremented automatically by the system.
// *
// * A client can subscribe to updates of the clock characteristics and get
// * notified when one of the value is changed. Clients can also change value of
// * the second, minute and hour characteristric.
// */
//class ClockService {
//    typedef ClockService Self;

//public:
//    ClockService() :
//        _hour_char("485f4145-52b9-4644-af1f-7a6b9322490f", 0),
//        _minute_char("0a924ca7-87cd-4699-a3bd-abdcd9cf126a", 0),
//        _second_char("8dd6a1b7-bc75-4741-8a26-264af75807de", 0),
//        _clock_service(
//            /* uuid */ "51311102-030e-485f-b122-f8f381aa84ed",
//            /* characteristics */ _clock_characteristics,
//            /* numCharacteristics */ sizeof(_clock_characteristics) /
//                                     sizeof(_clock_characteristics[0])
//        ),
//        _server(NULL),
//        _event_queue(NULL)
//    {
//        // update internal pointers (value, descriptors and characteristics array)
//        _clock_characteristics[0] = &_hour_char;
//        _clock_characteristics[1] = &_minute_char;
//        _clock_characteristics[2] = &_second_char;

//    }



//    void start(BLE &ble_interface, events::EventQueue &event_queue)
//    {
//         if (_event_queue) {
//            return;
//        }

//        _server = &ble_interface.gattServer();
//        _event_queue = &event_queue;

//        // register the service
//        printf("Adding demo service\r\n");
//        ble_error_t err = _server->addService(_clock_service);

//        if (err) {
//            printf("Error %u during demo service registration.\r\n", err);
//            return;
//        }

//        // updates subscribtion handlers
//        _server->onUpdatesEnabled(as_cb(&Self::when_update_enabled));
//        _server->onUpdatesDisabled(as_cb(&Self::when_update_disabled));
//        _event_queue->call_every(1000 /* ms */, callback(this, &Self::increment_second));
//    }

//private:

//    /**
//     * Handler called after a client has subscribed to notification or indication.
//     *
//     * @param handle Handle of the characteristic value affected by the change.
//     */
//    void when_update_enabled(GattAttribute::Handle_t handle)
//    {
//        printf("update enabled on handle %d\r\n", handle);
//    }

//    /**
//     * Handler called after a client has cancelled his subscription from
//     * notification or indication.
//     *
//     * @param handle Handle of the characteristic value affected by the change.
//     */
//    void when_update_disabled(GattAttribute::Handle_t handle)
//    {
//        printf("update disabled on handle %d\r\n", handle);
//    }

//    void increment_second(void){
//      static uint8_t second = 0;
//      ble_error_t  err = _second_char.set(*_server, ++second);
//    }

//    void increment_minute(void){
//        //uint8_t minute = 0;
//       // ble_error_t err = _minute_char.set(*_server, minute);
//    }

//    void increment_hour(void){
//       // uint8_t hour = 0;
//        //ble_error_t err = _hour_char.set(*_server, hour);
//    }

//private:
//    /**
//     * Helper that construct an event handler from a member function of this
//     * instance.
//     */
//    template<typename Arg>
//    FunctionPointerWithContext<Arg> as_cb(void (Self::*member)(Arg))
//    {
//        return makeFunctionPointer(this, member);
//    }

//    /**
//     * Read, Write, Notify, Indicate  Characteristic declaration helper.
//     *
//     * @tparam T type of data held by the characteristic.
//     */
//    template<typename T>
//    class CustomCharacteristic : public GattCharacteristic {
//    public:
//        /**
//         * Construct a characteristic that can be read or written and emit
//         * notification or indication.
//         *
//         * @param[in] uuid The UUID of the characteristic.
//         * @param[in] initial_value Initial value contained by the characteristic.
//         */
//        CustomCharacteristic(const UUID & uuid, const T& initial_value) :
//            GattCharacteristic(
//                /* UUID */ uuid,
//                /* Initial value */ &_value,
//                /* Value size */ sizeof(_value),
//                /* Value capacity */ sizeof(_value),
//                /* Properties */
//                                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY,
//                /* Descriptors */ NULL,
//                /* Num descriptors */ 0,
//                /* variable len */ true
//            ),
//            _value(initial_value) {
//        }

//        /**
//         * Get the value of this characteristic.
//         *
//         * @param[in] server GattServer instance that contain the characteristic
//         * value.
//         * @param[in] dst Variable that will receive the characteristic value.
//         *
//         * @return BLE_ERROR_NONE in case of success or an appropriate error code.
//         */
//        ble_error_t get(GattServer &server, T& dst) const
//        {
//            uint16_t value_length = sizeof(dst);
//            return server.read(getValueHandle(), &dst, &value_length);
//        }

//        /**
//         * Assign a new value to this characteristic.
//         *
//         * @param[in] server GattServer instance that will receive the new value.
//         * @param[in] value The new value to set.
//         * @param[in] local_only Flag that determine if the change should be kept
//         * locally or forwarded to subscribed clients.
//         */
//        ble_error_t set(
//            GattServer &server, const uint8_t &value, bool local_only = false
//        ) const {
//            return server.write(getValueHandle(), &value, sizeof(value), local_only);
//        }

//    private:
//        uint8_t _value;
//    };

//    CustomCharacteristic<uint8_t> _hour_char;
//    CustomCharacteristic<uint8_t> _minute_char;
//    CustomCharacteristic<uint8_t> _second_char;

//    // list of the characteristics of the clock service
//    GattCharacteristic* _clock_characteristics[3];

//    // demo service
//    GattService _clock_service;

//    GattServer* _server;
//    events::EventQueue *_event_queue;
//};

//int main() {
//    BLE &ble_interface = BLE::Instance();
//    events::EventQueue event_queue;
//    ClockService demo_service;
//    BLEProcess ble_process(event_queue, ble_interface);

//    ble_process.on_init(callback(&demo_service, &ClockService::start));

//    // bind the event queue to the ble interface, initialize the interface
//    // and start advertising
//    ble_process.start();

//    // Process the event queue.
//    event_queue.dispatch_forever();

//    return 0;
//}
