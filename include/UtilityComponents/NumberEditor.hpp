#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

template <typename T>
class IncDecButtons : public Component, public Button::Listener {
public:
    class Listener {
    public:
        Listener() = default;
        Listener(const Listener& rhs) = default;
        Listener& operator=(const Listener& rhs) = default;
        Listener(Listener&& rhs) noexcept = default;
        Listener& operator=(Listener&& rhs) noexcept = default;
        virtual ~Listener() noexcept = default;

        virtual void apply_increment(IncDecButtons* b, T value) = 0;
    };

    IncDecButtons() {
        addAndMakeVisible(sub_button);
        addAndMakeVisible(add_button);

        sub_button.addListener(this);
        add_button.addListener(this);

        sub_button.addMouseListener(this, false);
        add_button.addMouseListener(this, false);

        sub_button.setWantsKeyboardFocus(false);
        add_button.setWantsKeyboardFocus(false);
    }

    void resized() override {
        auto width = getWidth();
        auto button_width = (width - 2) / 2;

        sub_button.setSize(button_width, getHeight());
        add_button.setSize(button_width, getHeight());

        sub_button.setTopLeftPosition(0, 0);
        add_button.setTopLeftPosition(sub_button.getRight() + 2, 0);
    }

    void set_increment(T t) {
        increment = t;
    }

    T get_increment() const {
        return increment;
    }

    void addListener(Listener* l) {
        listener_list.add(l);
    }
    void removeListener(Listener* l) {
        listener_list.remove(l);
    }

    void mouseDown(const MouseEvent& e) override {
        dragged = false;
        mouse_start_pos = mouse_pos_when_last_dragged = e.position;
        if (isEnabled()) {
            value_on_mouse_down = value_on_prev_frame =
                    value_when_last_dragged = 0;
            mouseDrag(e);
        }
    }

    void handleDrag(const MouseEvent& e) {
        auto mouse_diff = e.position.x - mouse_start_pos.x;

        constexpr auto pixels_for_single_increment = 50;
        value_when_last_dragged =
                increment * mouse_diff / pixels_for_single_increment;

        auto since_last = e.position.x - mouse_pos_when_last_dragged.x;

        add_button.setState(since_last < 0 ? Button::buttonNormal
                                           : Button::buttonOver);
        sub_button.setState(since_last > 0 ? Button::buttonNormal
                                           : Button::buttonOver);

        e.source.enableUnboundedMouseMovement(true, false);
    }

    void mouseDrag(const MouseEvent& e) override {
        if (isEnabled()) {
            if (!dragged) {
                if (e.getDistanceFromDragStart() < 10 ||
                    !e.mouseWasDraggedSinceMouseDown()) {
                    return;
                }

                dragged = true;
                mouse_start_pos = e.position;
            }

            handleDrag(e);

            listener_list.call(&Listener::apply_increment,
                               this,
                               value_when_last_dragged - value_on_prev_frame);
            value_on_prev_frame = value_when_last_dragged;

            mouse_pos_when_last_dragged = e.position;
        }
    }

    void mouseUp(const MouseEvent& e) override {
        if (isEnabled()) {
            add_button.setState(Button::buttonNormal);
            sub_button.setState(Button::buttonNormal);
        }
    }

    void buttonClicked(Button* b) override {
        listener_list.call(&Listener::apply_increment,
                           this,
                           b == &add_button ? increment : -increment);
    }

private:
    TextButton sub_button{"-"};
    TextButton add_button{"+"};

    T increment{1};

    bool dragged{false};
    Point<float> mouse_start_pos;
    Point<float> mouse_pos_when_last_dragged;
    T value_on_mouse_down;
    T value_on_prev_frame;
    T value_when_last_dragged;

    ListenerList<Listener> listener_list;
};

template <typename T>
class NumberEditor : public Component,
                     public TextEditor::Listener,
                     public IncDecButtons<T>::Listener {
public:
    class Listener {
    public:
        Listener() = default;
        Listener(const Listener& rhs) = default;
        Listener& operator=(const Listener& rhs) = default;
        Listener(Listener&& rhs) noexcept = default;
        Listener& operator=(Listener&& rhs) noexcept = default;
        virtual ~Listener() noexcept = default;

        virtual void number_editor_value_changed(NumberEditor* e) = 0;
    };

    NumberEditor()
            : set_value_processor(
                      std::make_unique<PassthroughValueProcessor>()) {
        text_editor.setInputRestrictions(0, "0123456789.-+eE");
        text_editor.setSelectAllWhenFocused(true);
        text_editor.setWantsKeyboardFocus(true);

        addAndMakeVisible(text_editor);
        addAndMakeVisible(inc_dec_buttons);

        text_editor.addListener(this);
        inc_dec_buttons.addListener(this);

        set_text(0, false);
    }

    void resized() override {
        auto button_width = 52;
        auto bounds = getLocalBounds();

        text_editor.setBounds(bounds.withTrimmedRight(button_width + 2));
        inc_dec_buttons.setBounds(bounds.removeFromRight(button_width));
    }

    void apply_increment(IncDecButtons<T>* idb, T value) override {
        if (idb == &inc_dec_buttons) {
            set_value(get_value() + value, true);
        }
    }

    void textEditorEscapeKeyPressed(TextEditor& editor) override {
        if (&editor == &text_editor) {
            set_value(get_value(), false);
        }
    }

    void textEditorReturnKeyPressed(TextEditor& editor) override {
        if (&editor == &text_editor) {
            try {
                set_value(std::stof(editor.getText().toStdString()), true);
            } catch (const std::invalid_argument& e) {
                set_value(0, true);
            } catch (std::out_of_range& e) {
                set_value(0, true);
            }
        }
    }

    void textEditorFocusLost(TextEditor& editor) override {
        textEditorReturnKeyPressed(editor);
        editor.setHighlightedRegion(Range<int>(0, 0));
    }

    void set_value(T x, bool send_changed) {
        set_text(set_value_processor->process(x), false);
        if (send_changed) {
            listener_list.call(&Listener::number_editor_value_changed, this);
        }
    }
    T get_value() const {
        return value;
    }

    void addListener(Listener* l) {
        listener_list.add(l);
    }
    void removeListener(Listener* l) {
        listener_list.remove(l);
    }

    void set_increment(T t) {
        inc_dec_buttons.set_increment(t);
    }

    T get_increment() const {
        return inc_dec_buttons.get_increment();
    }

    void set_clipping(const Range<T>& r) {
        set_value_processor = std::make_unique<ClippingValueProcessor>(r);
    }

    void set_non_clipping() {
        set_value_processor = std::make_unique<PassthroughValueProcessor>();
    }

private:
    void set_text(T x, bool send_changed) {
        value = x;

        std::stringstream ss;
        ss << value;
        text_editor.setText(ss.str(), send_changed);
    }

    struct SetValueProcessor {
        virtual ~SetValueProcessor() noexcept = default;
        virtual T process(T in) const = 0;
    };

    struct PassthroughValueProcessor : public SetValueProcessor {
        T process(T in) const override {
            return in;
        }
    };

    struct ClippingValueProcessor : public SetValueProcessor {
        ClippingValueProcessor(const Range<T>& range)
                : range(range) {
        }
        T process(T in) const override {
            return range.clipValue(in);
        }

    private:
        Range<T> range;
    };

    std::unique_ptr<SetValueProcessor> set_value_processor;

    TextEditor text_editor;
    IncDecButtons<T> inc_dec_buttons;

    T value{0};

    ListenerList<Listener> listener_list;
};