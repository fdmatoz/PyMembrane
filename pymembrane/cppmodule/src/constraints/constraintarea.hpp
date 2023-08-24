#ifndef __constraintarea_hpp__
#define __constraintarea_hpp__

#include "../system/systemclass.hpp"
#include "constraintclass.hpp"

/**
 * @class ConstraintArea 
 * @brief Area constraint class
 * 
 */
class ConstraintArea : public ConstraintClass
{
public:
    ConstraintArea(SystemClass &system) : ConstraintClass(system)
    {
        name = "area";
        this->set_default_properties();
    }

    ~ConstraintArea() {}

    void set_default_properties(void) override
    {
        grad.resize(_system.Numvertices);
        ref_grad = grad;
        this->compute_ref_gradient();
    }

    void set_property(std::map<std::string, std::string> &region_map) override
    {
        for (const auto &item : region_map)
        {
            auto propname = item.first;
            auto value = item.second;
            if (propname.compare("A") == 0)
            {
                auto user_volumne = util::from_string_double(value);
                if (user_volumne > 0.0)
                {
                    m_target = user_volumne;
                }
                else
                {
                    std::string message = "Negative area constraint. Targeted volume in the volume constraint has to be larger then zero.";
                    this->print_warning_calling_err(message);
                }
                if (!_system.close_surface)
                {
                    std::string message = "Tried to constrain volume for an open system. It makes no sense to constrain volume for an open system. System type has to be \"vesicle\".";
                    this->print_warning_calling_err(message);

                }
            }
            else
                this->set_property_common(region_map);
        }
    }

    void enforce(void) override;
    void compute_ref_gradient(void) override;
    void compute_gradient(void) override;

private:
    pymemb::vector<real3> grad;
    pymemb::vector<real3> ref_grad;
};

#endif