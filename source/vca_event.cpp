//
// Created by root on 2019/12/2.
//
#include "../include/vca_event.h"

#include <utility>

Listener::Listener() {

}

Listener::~Listener() {

}

void Listener::listenObject(VBox::COMPtr<VBox::IVirtualBox> vbox) {

    VBox::COMPtr<VBox::IEventSource> source;
    VBox::COMPtr<VBox::IEventListener> listener;
    VBox::COMPtr<VBox::IVirtualBox> current_vbox;

    current_vbox = std::move(vbox);
    source = current_vbox->eventSource();
    listener = source->createListener();

    source->registerListener(listener, {VBox::VBoxEventType::MachineEvent}, false);

    std::thread event_thread([source, listener]() {
        for (;;) {
            auto event = source->getEvent(listener, 500);
            if (!event)
                continue;

//            std::wcout << L"Got event type " << static_cast<int>(event->type())<< std::endl;

            if (auto stateChangedEvent = event->QueryInterface<VBox::IMachineStateChangedEvent>()) {
                std::wcout << L"    State change: "
                           << static_cast<int>(stateChangedEvent->state())
                           << std::endl;
                if(static_cast<int>(static_cast<int>(stateChangedEvent->state()) == 5)){
                    break;
                }
            }
            source->eventProcessed(listener, event);
        }
    });

    event_thread.detach();
}