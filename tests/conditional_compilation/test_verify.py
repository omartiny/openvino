#!/usr/bin/env python3
# Copyright (C) 2021 Intel Corporation
# SPDX-License-Identifier: Apache-2.0

""" Test to verify inference results.
"""

import numpy as np
from tests_utils import run_infer


def test_verify(test_id, model, artifacts, openvino_root_dir, tolerance=1e-6):  # pylint: disable=too-many-arguments
    """ Test verifying that inference results are equal
    """
    out = artifacts / test_id
    install_prefix = artifacts / test_id / "install_pkg"
    returncode, output = run_infer(model, f"{out}.npz", openvino_root_dir)
    assert returncode == 0, f"Command exited with non-zero status {returncode}:\n {output}"
    returncode, output = run_infer(model, f"{out}_cc.npz", install_prefix)
    assert returncode == 0, f"Command exited with non-zero status {returncode}:\n {output}"
    reference_results = dict(np.load(f"{out}.npz"))
    inference_results = dict(np.load(f"{out}_cc.npz"))
    assert sorted(reference_results.keys()) == sorted(inference_results.keys()), \
        "Results have different number of layers"
    for layer in reference_results.keys():
        assert np.allclose(reference_results[layer], inference_results[layer], tolerance), \
            "Reference and inference results differ"
