/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "xtensor/xarray.hpp"

#include "defines.h"

namespace cvedia {
	namespace rt {

		class Tensor {
			enum datatype
			{
				INT,
				FLOAT,
				DOUBLE
			};

		public:
			EXPORT Tensor() = default;

			// Moves an xarray<float> into a Tensor
			EXPORT Tensor(xt::xarray<float>& t);
			// Moves an xarray<uint8_t> into a Tensor
			EXPORT Tensor(xt::xarray<uint8_t>& t);
			// Moves an xarray<double> into a Tensor
			EXPORT Tensor(xt::xarray<double>& t);

			Tensor(Tensor const& other) = delete;
			Tensor& operator=(Tensor const& other) = delete;
			EXPORT Tensor(Tensor&& other);
			Tensor& operator=(Tensor&& other) = delete;

			EXPORT void moveFrom(xt::xarray<float>& t);
			EXPORT void moveFrom(xt::xarray<uint8_t>& t);
			EXPORT void moveFrom(xt::xarray<double>& t);

			EXPORT void copyFrom(xt::xarray<float> const& t);
			EXPORT void copyFrom(xt::xarray<uint8_t> const& t);
			EXPORT void copyFrom(xt::xarray<double> const& t);

			template<typename T>
			EXPORT xt::xarray<T>& move();
		private:

			datatype type_ = datatype::FLOAT;

			xt::xarray<float> xtfloat_;
			xt::xarray<uint8_t> xtint_;
			xt::xarray<double> xtdouble_;
		};
	}
}