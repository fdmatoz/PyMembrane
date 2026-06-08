#ifndef __constraintvolume_hpp__
#define __constraintvolume_hpp__

#include "../system/systemclass.hpp"
#include "constraintclass.hpp"

/**
 * @class ConstraintVolume 
 * @brief Volume constraint class
 * 
 */
class ConstraintVolume : public ConstraintClass
{
public:
    ConstraintVolume(SystemClass &system) : ConstraintClass(system)
    {
        name = "volume";
        this->set_default_properties();
    }

    ~ConstraintVolume() {}

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
            if (propname.compare("V") == 0)
            {
                auto user_volumne = util::from_string_double(value);
                if (user_volumne > 0.0)
                {
                    m_target = user_volumne;
                }
                else
                {
                    std::string message = "Negative volume constraint. Targeted volume in the volume constraint has to be larger then zero.";
                    this->print_warning_calling_err(message);
                }
                if (!_system.close_surface)
                {
                    std::string message = "Tried to constrain volume for an open system.It makes no sense to constrain volume for an open system. System type has to be \"vesicle\".";
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