/**
 * Basic tool library
 * Copyright (C) 2014 kunyang  kunyang.@gmail.com.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file     bs_intrin.h
 * @brief    CPU的加速指令引用
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.0.1
 * @date     2011/05/20
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2011/05/02 | 1.0.0.1   | kunyang  | 创建文件
 */
#ifndef BS_INTRIN
#define BS_INTRIN
#include "bs_main.h"

#ifdef __AVX__ ///< -mavx	AVX: Advanced Vector   Extensions
#define bsHAS_AVX 1
#include <immintrin.h>
#endif

#ifdef __AVX2__ ///< -mavx2	AVX2
#define bsHAS_AVX2 1
#include <immintrin.h>
#endif

#ifdef __BMI__ ///< -mbmi	BMI: Bit Manipulation   Instruction
#define bsHAS_BMI 1
#include <bmiintrin.h>
#endif

#ifdef __BMI2__ ///< -mbmi2	BMI2
#define bsHAS_BMI2 1
#include <bmi2intrin.h>
#endif

#ifdef __MMX__ ///< -mmmx	MMX: MultiMedia eXtension
#define bsHAS_MMX 1
#include <mmintrin.h>
#endif

#ifdef __SSE2__ ///< -msse2	SSE2
#define bsHAS_SSE2 1
#include <emmintrin.h>
#endif

#ifdef __SSE3__ ///< -msse3	SSE3
#define bsHAS_SSE3 1
#include <pmmintrin.h>
#endif

///< -msse4	SSE4.1, SSE4.2
#ifdef __SSE4_1__
#define bsHAS_SSE4_1 1
#include <smmintrin.h>
#endif

#ifdef __SSE4_2__ ///< -msse4.2	SSE4.2
#define bsHAS_SSE4_2 1
#include <smmintrin.h>
#include <nmmintrin.h>
#endif

#ifdef __SSE4A__ ///< -msse4a	SSE4A
#define bsHAS_SSE4A 1
#include <ammintrin.h>
#endif

#ifdef __F16C__ ///< -mf16c	F16C
#define bsHAS_F16C 1
#include <f16cintrin.h>
#endif

#ifdef __FMA__ ///< -mfma	FMA
#define bsHAS_FMA 1
#include <fmaintrin.h>
#endif

#ifdef __FMA4__ ///< -mfma4	FMA4
#define bsHAS_FMA4 1
#include <x86intrin.h>
#endif

///< -mrdrnd -mfsgsbase	Intel提出的扩展指令.
#ifdef __RDRND__
#define bsHAS_RDRND 1
#include <immintrin.h>
#endif
#ifdef __FSGSBASE__
#define bsHAS_FSGSBASE 1
#include <immintrin.h>
#endif

#ifdef __LWP__ ///< -mlwp	LWP: Light Weight Profiling
#define bsHAS_LWP 1
#include <lwpintrin.h>
#endif

#ifdef __LZCNT__ ///< -mlzcnt	LZCNT
#define bsHAS_LZCNT 1
#include <x86intrin.h>
#endif

#ifdef __3dNOW__ ///< -m3dnow	3DNow!
#define bsHAS_3dNOW 1
#include <mm3dnow.h>
#endif

#ifdef __POPCNT__ ///< -mpopcnt	POPCNT
#define bsHAS_POPCNT 1
#include <popcntintrin.h>
#endif

#ifdef __TBM__ ///< -mtbm	TBM: Trailing bit manipulation.
#define bsHAS_TBM 1
#include <tbmintrin.h>
#endif

#ifdef __SSSE3__ ///< -mssse3	SSSE3
#define bsHAS_SSSE3 1
#include <tmmintrin.h>
#endif

///< -maes -mpclmul	AES, PCLMUL
#ifdef __AES__
#define bsHAS_AES 1
#include <wmmintrin.h>
#endif

#ifdef __PCLMUL__
#define bsHAS_PCLMUL 1
#include <wmmintrin.h>
#endif

#ifdef __SSE__ ///< -msse	SSE: Streaming SIMD Extensions
#define bsHAS_SSE 1
#include <xmmintrin.h>
#endif

#ifdef __XOP__ ///< -mxop	XOP
#define bsHAS_XOP 1
#include <xopintrin.h>
#endif

///< bsHAS_INTRIN 是否开启支持CPU扩展指令
#define bsHAS_INTRIN 1

#endif // BS_INTRIN

