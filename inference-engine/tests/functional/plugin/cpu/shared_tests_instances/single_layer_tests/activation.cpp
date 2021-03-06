// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <vector>
#include "single_layer_tests/activation.hpp"
#include "common_test_utils/test_constants.hpp"

using namespace LayerTestsDefinitions;
using namespace ngraph::helpers;
namespace {
// Common params
const std::vector<InferenceEngine::Precision> inputPrecisions = {
        InferenceEngine::Precision::FP32
        // TODO: Fix Issue-27390
        // InferenceEngine::Precision::I16,
        // InferenceEngine::Precision::U8
};

const std::vector<InferenceEngine::Precision> netPrecisions = {
        InferenceEngine::Precision::FP32,
        InferenceEngine::Precision::FP16
};

const std::vector<InferenceEngine::Precision> intPrecisions = {
        InferenceEngine::Precision::I32,
};

const std::map<ActivationTypes, std::vector<std::vector<float>>> activationTypes = {
        {Sigmoid,               {}},
        {Tanh,                  {}},
        {Relu,                  {}},
        {Exp,                   {}},
        {Log,                   {}},
        {Sign,                  {}},
        {Abs,                   {}},
        {Clamp,                 {{-2.0f, 2.0f}}},
        {Negative,              {}},
        {Acos,                  {}},
        {Asin,                  {}},
        {Atan,                  {}},
        {Cos,                   {}},
        {Cosh,                  {}},
        {Floor,                 {}},
        {Sin,                   {}},
        {Sinh,                  {}},
        {Sqrt,                  {}},
        {Tan,                   {}},
        {Elu,                   {{0.1f}}},
        {Erf,                   {}},
        {HardSigmoid,           {{0.2f, 0.5f}}},
        {Selu,                  {{1.6732f, 1.0507f}}},
        {Ceiling,               {}},
        {Mish,                  {}},
        {HSwish,                {}},
        {SoftPlus,              {}},
        {HSigmoid,              {}},
        {RoundHalfToEven,       {}},
        {RoundHalfAwayFromZero, {}},
        {Erf,                   {}},
        {GeluErf,               {}},
        {GeluTanh,              {}}
};

// List of operations that should be tested also with integer precision
const std::map<ActivationTypes, std::vector<std::vector<float>>> intActivationTypes = {
        {Sqrt,                  {}},
        {Tanh,                  {}},
};

const std::map<ActivationTypes, std::vector<std::vector<float>>> activationParamTypes = {
    {PReLu, {{-0.01f}}},
    {LeakyRelu, {{0.01f}}}
};

std::map<std::vector<size_t>, std::vector<std::vector<size_t>>> basic = {
        {{1, 50}, {{}}},
        {{1, 128}, {{}}},
};

std::map<std::vector<size_t>, std::vector<std::vector<size_t>>> preluBasic = {
        {{1, 50}, {{1}, {50}}},
        {{1, 128}, {{1}, {128}}},
        {{20, 128}, {{128}}},
        {{1, 20, 128}, {{1}, {20}}},
        {{1, 20, 128, 128}, {{1}, {20}}},
        {{1, 20, 20, 128, 128}, {{1}, {20}}}
        // according to spec second input for PRelu must be 1D and must be broadcastabe per channel
        // at this moment these cases unsupported
        // {{20, 128}, {{20}, {20, 128}}},
        // {{1, 20, 128}, {{128}, {20, 128}}},
        // {{1, 20, 128, 128}, {{128}, {128, 128}, {20, 128, 128}}},
        // {{1, 20, 20, 128, 128}, {{128}, {128, 128}, {20, 128, 128}, {20, 20, 128, 128}}},
};

const auto basicCases = ::testing::Combine(
        ::testing::ValuesIn(CommonTestUtils::combineParams(activationTypes)),
        ::testing::ValuesIn(netPrecisions),
        ::testing::Values(InferenceEngine::Precision::UNSPECIFIED),
        ::testing::Values(InferenceEngine::Precision::UNSPECIFIED),
        ::testing::Values(InferenceEngine::Layout::ANY),
        ::testing::Values(InferenceEngine::Layout::ANY),
        ::testing::ValuesIn(CommonTestUtils::combineParams(basic)),
        ::testing::Values(CommonTestUtils::DEVICE_CPU)
);

const auto basicPreluCases = ::testing::Combine(
        ::testing::ValuesIn(CommonTestUtils::combineParams(activationParamTypes)),
        ::testing::ValuesIn(netPrecisions),
        ::testing::Values(InferenceEngine::Precision::UNSPECIFIED),
        ::testing::Values(InferenceEngine::Precision::UNSPECIFIED),
        ::testing::Values(InferenceEngine::Layout::ANY),
        ::testing::Values(InferenceEngine::Layout::ANY),
        ::testing::ValuesIn(CommonTestUtils::combineParams(preluBasic)),
        ::testing::Values(CommonTestUtils::DEVICE_CPU)
);

const auto basicIntegerOperations = ::testing::Combine(
            ::testing::ValuesIn(CommonTestUtils::combineParams(intActivationTypes)),
            ::testing::ValuesIn(intPrecisions),
            ::testing::ValuesIn(intPrecisions),
            ::testing::ValuesIn(intPrecisions),
            ::testing::Values(InferenceEngine::Layout::ANY),
            ::testing::Values(InferenceEngine::Layout::ANY),
            ::testing::ValuesIn(CommonTestUtils::combineParams(basic)),
            ::testing::Values(CommonTestUtils::DEVICE_CPU)
);

INSTANTIATE_TEST_CASE_P(smoke_Activation_Basic, ActivationLayerTest, basicCases, ActivationLayerTest::getTestCaseName);
INSTANTIATE_TEST_CASE_P(smoke_Integer_Activation_Basic, ActivationLayerTest, basicIntegerOperations, ActivationLayerTest::getTestCaseName);
INSTANTIATE_TEST_CASE_P(smoke_Activation_Basic_Prelu, ActivationLayerTest, basicPreluCases, ActivationLayerTest::getTestCaseName);

INSTANTIATE_TEST_CASE_P(smoke_Activation_Basic, ActivationParamLayerTest, basicPreluCases, ActivationLayerTest::getTestCaseName);

INSTANTIATE_TEST_CASE_P(smoke_Activation_Basic, ActivationDynamicLayerTest, basicCases, ActivationLayerTest::getTestCaseName);

}  // namespace
