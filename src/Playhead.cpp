#include "UtilityComponents/Playhead.hpp"

void Playhead::paint(Graphics& g) {
    g.setColour(Colours::red);
    g.drawVerticalLine(getWidth() * 0.5, 0, getHeight());
}

void Playhead::mouseEnter(const MouseEvent& e) {
    setMouseCursor(MouseCursor::LeftRightResizeCursor);
}

void Playhead::mouseExit(const MouseEvent& e) {
    setMouseCursor(MouseCursor::NormalCursor);
}

void Playhead::mouseDrag(const MouseEvent& e) {
    listener_list.call(&Listener::playhead_dragged, this, e);
}

void Playhead::addListener(Listener* l) {
    listener_list.add(l);
}

void Playhead::removeListener(Listener* l) {
    listener_list.remove(l);
}
