/************************************************************************************
 * MIT License                                                                       *
 *                                                                                   *
 * Copyright (c) 2023 Dr. Daniel Alejandro Matoz Fernandez                           *
 *               fdamatoz@gmail.com                                                  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy      *
 * of this software and associated documentation files (the "Software"), to deal     *
 * in the Software without restriction, including without limitation the rights      *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell         *
 * copies of the Software, and to permit persons to whom the Software is             *
 * furnished to do so, subject to the following conditions:                          *
 *                                                                                   *
 * The above copyright notice and this permission notice shall be included in all    *
 * copies or substantial portions of the Software.                                   *
 *                                                                                   *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER            *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,     *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     *
 * SOFTWARE.                                                                         *
 *************************************************************************************/
#ifndef __integrator_verlet_vertex__hpp__
#define __integrator_verlet_vertex__hpp__

#include <iostream>

#include "integratorclass.hpp"

/**
 * @class IntegratorVelocityVerletMeshVertex
 * @brief Integrator IntegratorVelocityVerletMeshVertex class implements
 * the Velocity Verlet dynamics for the vertices position.
 */
class IntegratorVelocityVerletMeshVertex : public IntegratorClass
{
public:
    /** @brief VertexIntegrator Constructor */
    IntegratorVelocityVerletMeshVertex(SystemClass &system) : IntegratorClass(system)
    {
        m_name = "brownian";
        m_type = "vertex";
        this->set_default_properties();
    }
    /** @brief destructor */
    ~IntegratorVelocityVerletMeshVertex() {}

    void set_default_properties(void) override
    {
        m_limit_val = 0.1;
        m_has_limit = false;
        this->set_temperature(0.0);
        this->set_time_step(5e-3);
        this->update_temperature_parameters();
        this->update_time_step_parameters();
    }

    /** @brief Update the temperature dependent parameters **/
    void update_temperature_parameters() override {}
    /** @brief Update the temperature dependent parameters **/
    void update_time_step_parameters() override {}

    using IntegratorClass::set_property;
    void set_property(std::map<std::string, std::string> &value_map) override
    {
        for (const auto &item : value_map)
        {
            if (item.first.compare("T") == 0)
            {
                this->set_temperature(util::from_string_double(item.second));
                this->update_temperature_parameters();
            }
            else if (item.first.compare("limit") == 0)
            {
                m_has_limit = util::from_string_bool(item.second);
            }
            else if (item.first.compare("limit_val") == 0)
            {
                m_has_limit = true;
                m_limit_val = util::from_string_double(item.second);
            }
            else
                print_warning_property_name(item.first);
        }
    }
    std::map<std::string, std::string> get_info(void) override
    {
        std::map<std::string, std::string> value;
        value["name"] = m_name;
        value["type"] = m_type;
        value["T"] = util::to_string(this->get_temperature());
        value["dt"] = util::to_string(this->get_time_step());
        value["m_has_limit"] = util::to_string(this->m_has_limit);
        value["m_limit_val"] = util::to_string(this->m_limit_val);
        return value;
    }
    /**  @brief Propagate system for a time step */
    void prestep(void) override;

    void poststep(void) override;

private:
    real m_limit_val; //!< Maximum vertex mobility (1/gamma)
    bool m_has_limit; //!< set the limit on/off
};

#endif
