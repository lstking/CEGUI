/***********************************************************************
    filename:   SVGTesselator.cpp
    created:    1st August 2013
    author:     Lukas Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/svg/SVGTesselator.h"

#include "CEGUI/svg/SVGBasicShape.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/System.h"
#include "CEGUI/Vertex.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderPolyline(const SVGPolyline* polyline,
                                               std::vector<GeometryBuffer*>& geometry_buffers,
                                               const SVGImage::SVGImageRenderSettings& render_settings)
{
    CEGUI::GeometryBuffer& geometry_buffer = System::getSingleton().getRenderer()->createGeometryBufferColoured();
    geometry_buffers.push_back(&geometry_buffer);

    if(render_settings.d_clipArea)
    {
        geometry_buffer.setClippingActive(true);
        geometry_buffer.setClippingRegion(*render_settings.d_clipArea);
    }
    geometry_buffer.setScale(render_settings.d_scaleFactor);

    const glm::vec3& strokeColourValues = polyline->d_paintStyle.d_stroke.d_colour;
    const CEGUI::Colour stroke_colour(strokeColourValues.x, strokeColourValues.y, strokeColourValues.z, polyline->d_paintStyle.d_strokeOpacity);
    const SVGPolyline::PolylinePointsList& points = polyline->d_points;
    const float& stroke_width_length_half = polyline->d_paintStyle.d_strokeWidth * 0.5f;

    //Draw the line segments TODO Ident: Draw them seamlessly connected with bevels and stuff provided optionally
    size_t points_count = points.size();
    for (size_t j = 1; j < points_count; ++j)
    {
        const glm::vec2& prevPos = points.at(j - 1);
        const glm::vec2& currentPos = points.at(j);

        // Normalize and tilt the 2D direction vector by 90� to get the vector pointing in the offset direction
        glm::vec2 offsetVector = currentPos - prevPos;
        offsetVector = glm::normalize(offsetVector);
        offsetVector = glm::vec2(offsetVector.y, -offsetVector.x) * stroke_width_length_half;

        CEGUI::ColouredVertex linePositionVertex;
        glm::vec2 vertexPosition;
        linePositionVertex.colour_val = stroke_colour;

        linePositionVertex.position = glm::vec3(prevPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(prevPos - offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(prevPos + offsetVector, 0.0f);
        geometry_buffer.appendVertex(linePositionVertex);
    }
}


//----------------------------------------------------------------------------//
void SVGTesselator::tesselateAndRenderRect(const SVGRect* rect,
                                           std::vector<GeometryBuffer*>& geometry_buffers,
                                           const SVGImage::SVGImageRenderSettings& render_settings)
{
    CEGUI::GeometryBuffer& geometry_buffer = System::getSingleton().getRenderer()->createGeometryBufferColoured();
    geometry_buffers.push_back(&geometry_buffer);

    if(render_settings.d_clipArea)
    {
        geometry_buffer.setClippingActive(true);
        geometry_buffer.setClippingRegion(*render_settings.d_clipArea);
    }

    geometry_buffer.setScale(render_settings.d_scaleFactor);

    const glm::vec3& colourValues = rect->d_paintStyle.d_fill.d_colour;
    const CEGUI::Colour fill_colour(colourValues.x, colourValues.y, colourValues.z, rect->d_paintStyle.d_fillOpacity);

    //Draw the rectangle
    CEGUI::ColouredVertex rectFillVertex;
    glm::vec2 vertexPosition;
    rectFillVertex.colour_val = fill_colour;

    const float& rectX1 = rect->d_x;
    const float& rectY1 = rect->d_y;
    const float rectX2 = rect->d_x + rect->d_width;
    const float rectY2 = rect->d_y + rect->d_height;

    rectFillVertex.position = glm::vec3(rectX1, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX1, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX2, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX2, rectY2, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX1, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);

    rectFillVertex.position = glm::vec3(rectX2, rectY1, 0.0f);
    geometry_buffer.appendVertex(rectFillVertex);
}
//----------------------------------------------------------------------------//
}
