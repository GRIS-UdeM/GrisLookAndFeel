/*
 ==============================================================================
 
 This file is part of the JUCE library.
 Copyright (c) 2015 - ROLI Ltd.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses JUCE, commercial licenses are
 available: visit www.juce.com for more information.
 
 ==============================================================================
 */

#ifndef GRISLOOKANDFEEL_H_INCLUDED
#define GRISLOOKANDFEEL_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"



//==============================================================================
/** Custom Look And Feel subclasss.
 
 Simply override the methods you need to, anything else will be inherited from the base class.
 It's a good idea not to hard code your colours, use the findColour method along with appropriate
 ColourIds so you can set these on a per-component basis.
 */
class GrisLookAndFeel    : public LookAndFeel_V3 {
private:
    
    Font m_Font;
    float m_fFontSize;
    Colour m_BackGroundAndFieldColor;
public:
    GrisLookAndFeel(){
        m_BackGroundAndFieldColor = Colours::darkgrey;
        
        m_Font = Font(juce::CustomTypeface::createSystemTypefaceFor(BinaryData::SinkinSans400Regular_otf, (size_t) BinaryData::SinkinSans400Regular_otfSize));
        m_fFontSize = 10.f;
        m_Font.setHeight(m_fFontSize);
    }
    
    Font getLabelFont (Label & label) override{
        return m_Font;
    }
    Font getComboBoxFont (ComboBox & comboBox) override{
        return m_Font;
    }
    Font getTextButtonFont (TextButton &, int buttonHeight) override{
        return m_Font;
    }
    Font getMenuBarFont	(MenuBarComponent &, int itemIndex, const String & itemText) override{
        return m_Font;
    }
    Colour getBackgroundColor(){
        //        return Colours::darkblue;
        //        return Colours::dodgerblue;
        return m_BackGroundAndFieldColor;
    }
    
    Colour getFieldColor(){
        //        return Colours::darkblue;
        //        return Colours::dodgerblue;
        return m_BackGroundAndFieldColor;
    }
    
    Colour getFontColour(){
        //        return Colours::azure;
        return Colours::whitesmoke;
    }
    
//    Typeface::Ptr getTypefaceForFont (const Font & font) override{
//        return juce::CustomTypeface::createSystemTypefaceFor(BinaryData::SinkinSans400Regular_otf, (size_t) BinaryData::SinkinSans400Regular_otfSize);
//    }
    
    void drawRoundThumb (Graphics& g, const float x, const float y, const float diameter, const Colour& colour, float outlineThickness) {
        const juce::Rectangle<float> a (x, y, diameter, diameter);
        const float halfThickness = outlineThickness * 0.5f;
        
        Path p;
        p.addEllipse (x + halfThickness, y + halfThickness, diameter - outlineThickness, diameter - outlineThickness);
        
        const DropShadow ds (Colours::black, 1, Point<int> (0, 0));
        ds.drawForPath (g, p);
        
        g.setColour (colour);
        g.fillPath (p);
        
        g.setColour (colour.brighter());
        g.strokePath (p, PathStrokeType (outlineThickness));
    }
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override {
        Colour baseColour (backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f).withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.5f));
        
        if (isButtonDown || isMouseOverButton){
            baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);
        }
        const bool flatOnLeft   = button.isConnectedOnLeft();
        const bool flatOnRight  = button.isConnectedOnRight();
        const bool flatOnTop    = button.isConnectedOnTop();
        const bool flatOnBottom = button.isConnectedOnBottom();
        
        const float width  = button.getWidth() - 1.0f;
        const float height = button.getHeight() - 1.0f;
        
        if (width > 0 && height > 0) {
            const float cornerSize = jmin (15.0f, jmin (width, height) * 0.45f);
            const float lineThickness = cornerSize * 0.1f;
            const float halfThickness = lineThickness * 0.5f;
            
            Path outline;
            outline.addRoundedRectangle (0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
                                         cornerSize, cornerSize,
                                         ! (flatOnLeft  || flatOnTop),
                                         ! (flatOnRight || flatOnTop),
                                         ! (flatOnLeft  || flatOnBottom),
                                         ! (flatOnRight || flatOnBottom));
            
            const Colour outlineColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId : TextButton::textColourOffId));
            g.setColour (baseColour);
            g.fillPath (outline);
            
            if (! button.getToggleState()) {
                g.setColour (outlineColour);
                g.strokePath (outline, PathStrokeType (lineThickness));
            }
        }
    }
    
    void drawTickBox (Graphics& g, Component& component, float x, float y, float w, float h, bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown) override {
        const float boxSize = w * 0.7f;
        bool isDownOrDragging = component.isEnabled() && (component.isMouseOverOrDragging() || component.isMouseButtonDown());
        const Colour colour (component.findColour (TextButton::buttonColourId).withMultipliedSaturation ((component.hasKeyboardFocus (false) || isDownOrDragging) ? 1.3f : 0.9f).withMultipliedAlpha (component.isEnabled() ? 1.0f : 0.7f));
        
        drawRoundThumb (g, x, y + (h - boxSize) * 0.5f, boxSize, colour, isEnabled ? ((isButtonDown || isMouseOverButton) ? 1.1f : 0.5f) : 0.3f);
        
        if (ticked) {
            const Path tick (LookAndFeel_V2::getTickShape (6.0f));
            g.setColour (isEnabled ? findColour (TextButton::buttonOnColourId) : Colours::grey);
            const float scale = 9.0f;
            const AffineTransform trans (AffineTransform::scale (w / scale, h / scale).translated (x - 2.5f, y + 1.0f));
            g.fillPath (tick, trans);
        }
    }
    
    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override {
        const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
        bool isDownOrDragging = slider.isEnabled() && (slider.isMouseOverOrDragging() || slider.isMouseButtonDown());
        Colour knobColour (slider.findColour (Slider::thumbColourId).withMultipliedSaturation ((slider.hasKeyboardFocus (false) || isDownOrDragging) ? 1.3f : 0.9f).withMultipliedAlpha (slider.isEnabled() ? 1.0f : 0.7f));
        
        if (style == Slider::LinearHorizontal || style == Slider::LinearVertical) {
            float kx, ky;
            
            if (style == Slider::LinearVertical) {
                kx = x + width * 0.5f;
                ky = sliderPos;
            } else {
                kx = sliderPos;
                ky = y + height * 0.5f;
            }
            
            const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;
            
            drawRoundThumb (g,
                            kx - sliderRadius,
                            ky - sliderRadius,
                            sliderRadius * 2.0f,
                            knobColour, outlineThickness);
        } else {
            // Just call the base class for the demo
            LookAndFeel_V2::drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        }
    }
    
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) override {
        g.fillAll (slider.findColour (Slider::backgroundColourId));
        
        if (style == Slider::LinearBar || style == Slider::LinearBarVertical) {
            const float fx = (float) x, fy = (float) y, fw = (float) width, fh = (float) height;
            
            Path p;
            
            if (style == Slider::LinearBarVertical)
                p.addRectangle (fx, sliderPos, fw, 1.0f + fh - sliderPos);
                else
                    p.addRectangle (fx, fy, sliderPos - fx, fh);
                    
                    
                    Colour baseColour (slider.findColour (Slider::rotarySliderFillColourId)
                                       .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f)
                                       .withMultipliedAlpha (0.8f));
                    
                    g.setColour (baseColour);
                    g.fillPath (p);
                    
                    const float lineThickness = jmin (15.0f, jmin (width, height) * 0.45f) * 0.1f;
            g.drawRect (slider.getLocalBounds().toFloat(), lineThickness);
        } else {
            drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
            drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        }
    }
    
    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height, float /*sliderPos*/, float /*minSliderPos*/, float /*maxSliderPos*/, const Slider::SliderStyle /*style*/, Slider& slider) override {
        const float sliderRadius = getSliderThumbRadius (slider) - 5.0f;
        Path on, off;
        if (slider.isHorizontal()) {
            const float iy = y + height * 0.5f - sliderRadius * 0.5f;
            juce::Rectangle<float> r (x - sliderRadius * 0.5f, iy, width + sliderRadius, sliderRadius);
            const float onW = r.getWidth() * ((float) slider.valueToProportionOfLength (slider.getValue()));
            on.addRectangle (r.removeFromLeft (onW));
            off.addRectangle (r);
        } else {
            const float ix = x + width * 0.5f - sliderRadius * 0.5f;
            juce::Rectangle<float> r (ix, y - sliderRadius * 0.5f, sliderRadius, height + sliderRadius);
            const float onH = r.getHeight() * ((float) slider.valueToProportionOfLength (slider.getValue()));
            on.addRectangle (r.removeFromBottom (onH));
            off.addRectangle (r);
        }
        g.setColour (slider.findColour (Slider::rotarySliderFillColourId));
        g.fillPath (on);
        g.setColour (slider.findColour (Slider::trackColourId));
        g.fillPath (off);
    }
    
    void drawToggleButton (Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown) override {
        if (button.hasKeyboardFocus (true))
        {
            g.setColour (button.findColour (TextEditor::focusedOutlineColourId));
            g.drawRect (0, 0, button.getWidth(), button.getHeight());
        }
        
        float fontSize = jmin (15.0f, button.getHeight() * 0.75f);
        const float tickWidth = fontSize * 1.1f;
        
        drawTickBox (g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
                     tickWidth, tickWidth,
                     button.getToggleState(),
                     button.isEnabled(),
                     isMouseOverButton,
                     isButtonDown);
        
        g.setColour (button.findColour (ToggleButton::textColourId));
        g.setFont(m_Font);
//        g.setFont (fontSize);
        
        if (! button.isEnabled())
            g.setOpacity (0.5f);
        
        const int textX = (int) tickWidth + 5;
        
        g.drawFittedText (button.getButtonText(),
                          textX, 0,
                          button.getWidth() - textX - 2, button.getHeight(),
                          Justification::centredLeft, 10);
    }
    
    void drawTabButtonText (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override {
        const Rectangle<float> area (button.getTextArea().toFloat());
        
        float length = area.getWidth();
        float depth  = area.getHeight();
        
        if (button.getTabbedButtonBar().isVertical()){
            std::swap (length, depth);
        }
        
        //        Font font (depth * 0.6f);
        Font font(m_Font);
        font.setHeight(depth * 0.6f);
        cout << "SpatGris drawTabButtonText\n";
        font.setUnderline (button.hasKeyboardFocus (false));
        
        AffineTransform t;
        
        switch (button.getTabbedButtonBar().getOrientation()) {
            case TabbedButtonBar::TabsAtLeft:   t = t.rotated (float_Pi * -0.5f).translated (area.getX(), area.getBottom()); break;
            case TabbedButtonBar::TabsAtRight:  t = t.rotated (float_Pi *  0.5f).translated (area.getRight(), area.getY()); break;
            case TabbedButtonBar::TabsAtTop:
            case TabbedButtonBar::TabsAtBottom: t = t.translated (area.getX(), area.getY()); break;
            default:                            jassertfalse; break;
        }
        
        Colour col;
        
        if (button.isFrontTab() && (button.isColourSpecified (TabbedButtonBar::frontTextColourId) || isColourSpecified (TabbedButtonBar::frontTextColourId))){
            col = findColour (TabbedButtonBar::frontTextColourId);
        } else if (button.isColourSpecified (TabbedButtonBar::tabTextColourId) || isColourSpecified (TabbedButtonBar::tabTextColourId)){
            col = findColour (TabbedButtonBar::tabTextColourId);
        } else {
            col = button.getTabBackgroundColour().contrasting();
        }
        const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;
        g.setColour (col.withMultipliedAlpha (alpha));
        g.setFont (font);
        g.addTransform (t);
        g.drawFittedText (button.getButtonText().trim(), 0, 0, (int) length, (int) depth, Justification::centred, jmax (1, ((int) depth) / 12));
    }
        
//        int getTabButtonOverlap (int /*tabDepth*/)            override { return -1; }
//        int getTabButtonSpaceAroundImage()                    override { return 0; }

        
        //    //we don't use those, so far
        //    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override {
        //        const float radius = jmin (width / 2, height / 2) - 2.0f;
        //        const float centreX = x + width * 0.5f;
        //        const float centreY = y + height * 0.5f;
        //        const float rx = centreX - radius;
        //        const float ry = centreY - radius;
        //        const float rw = radius * 2.0f;
        //        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        //        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
        //
        //        if (slider.isEnabled())
        //            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        //        else
        //            g.setColour (Colour (0x80808080));
        //
        //        {
        //            Path filledArc;
        //            filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, 0.0);
        //            g.fillPath (filledArc);
        //        }
        //
        //        {
        //            const float lineThickness = jmin (15.0f, jmin (width, height) * 0.45f) * 0.1f;
        //            Path outlineArc;
        //            outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.0);
        //            g.strokePath (outlineArc, PathStrokeType (lineThickness));
        //        }
        //    }
};
        
#endif
        
