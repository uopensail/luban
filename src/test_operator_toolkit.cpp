#include "feature_operator_toolkit.hpp"

int main()
{
    std::shared_ptr<cpptoml::table> g = cpptoml::parse_file("config.toml");

    auto operators = g->get_table_array("operators");

    std::vector<ConfigureOperator> ops;
    for (const auto &table : *operators)
    {
        ops.push_back({table});
    }

    FeatureOperatorToolkit toolkit;
    SharedFeaturesPtr features(new tensorflow::Features());
    SharedFeaturePtr feature(new tensorflow::Feature());
    add_value<float>(feature, 0.6);
    (*features->mutable_feature())["i_item"] = *feature;
    RunTimeFeatures rt_feartures(features);
    toolkit.call(ops[0], rt_feartures);
    

    std::cout<<rt_feartures.get_selected()->DebugString()<<std::endl;
    return 0;
}