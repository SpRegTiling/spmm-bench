// Auto-generated file. Do not edit!
//   Template: src/f32-spmm/neon.c.in
//   Generator: tools/xngen
//
// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>
#include <stddef.h>
#include <arm_neon.h>

#include "SpMM_XNN.h"

//
// From: https://github.com/google/XNNPACK/blob/295ea1aaf511ee594f3500da8465d20ea024fec1/src/xnnpack/common.h
//      Removed minmax for fairness
//      This file is part of XNNPACK, a library for efficient neural network inference.
//

#if defined(__GNUC__)
#define XNN_LIKELY(condition) (__builtin_expect(!!(condition), 1))
#define XNN_UNLIKELY(condition) (__builtin_expect(!!(condition), 0))
#else
#define XNN_LIKELY(condition) (!!(condition))
  #define XNN_UNLIKELY(condition) (!!(condition))
#endif

#define restrict __restrict


//#define MIN_MAX

void xnn_f32_spmm_minmax_ukernel_16x1__neon(
        size_t mc,
        size_t nc,
        const float*restrict input,
        const float*restrict weights,
        const int32_t*restrict widx_dmap,
        const uint32_t*restrict nidx_nnzmap,
        float*restrict output,
        size_t output_stride
)
{
    assert(mc != 0);
    assert(mc % sizeof(float) == 0);
    assert(nc != 0);

    size_t m = mc / sizeof(float);

    size_t output_decrement = output_stride * nc - 16 * sizeof(float);
    while XNN_LIKELY(mc >= 16 * sizeof(float)) {
        const float*restrict w = weights;
        const int32_t* dmap = widx_dmap;
        const uint32_t* nnzmap = nidx_nnzmap;
        size_t n = nc;
        do {
            uint32_t nnz = *nnzmap++;
            float32x4_t vacc0123 = vld1q_dup_f32(w); w += 1;
            float32x4_t vacc4567 = vacc0123;
            float32x4_t vacc89AB = vacc0123;
            float32x4_t vaccCDEF = vacc0123;
            if (nnz != 0) {
                do {
                    const intptr_t diff = *dmap++;
                    const float32x4_t vi0123 = vld1q_f32(input);
                    const float32x4_t vi4567 = vld1q_f32(input + 4);
                    const float32x4_t vi89AB = vld1q_f32(input + 8);
                    const float32x4_t viCDEF = vld1q_f32(input + 12);
#ifdef PRESCALE_DIFF
                    input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
#else
                    input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff * (uintptr_t) m);
#endif
                    //__builtin_prefetch(input + 16);
                    const float32x4_t vw = vld1q_dup_f32(w); w += 1;
                    //__builtin_prefetch(w + 32);
                    vacc0123 = vmlaq_f32(vacc0123, vi0123, vw);
                    vacc4567 = vmlaq_f32(vacc4567, vi4567, vw);
                    vacc89AB = vmlaq_f32(vacc89AB, vi89AB, vw);
                    vaccCDEF = vmlaq_f32(vaccCDEF, viCDEF, vw);
                } while (--nnz != 0);
            }
            //float32x4_t vout0123 = vminq_f32(vacc0123, vmax);
            //float32x4_t vout4567 = vminq_f32(vacc4567, vmax);
            //float32x4_t vout89AB = vminq_f32(vacc89AB, vmax);
            //float32x4_t voutCDEF = vminq_f32(vaccCDEF, vmax);
            //vout0123 = vmaxq_f32(vout0123, vmin);
            //vout4567 = vmaxq_f32(vout4567, vmin);
            //vout89AB = vmaxq_f32(vout89AB, vmin);
            //voutCDEF = vmaxq_f32(voutCDEF, vmin);
            vst1q_f32(output, vacc0123);
            vst1q_f32(output + 4, vacc4567);
            vst1q_f32(output + 8, vacc89AB);
            vst1q_f32(output + 12, vaccCDEF);
            output = (float*restrict) ((uintptr_t) output + output_stride);
        } while (--n != 0);
        output = (float*restrict) ((uintptr_t) output - output_decrement);
        input += 16;
        mc -= 16 * sizeof(float);
    }
    if XNN_UNLIKELY(mc != 0) {
        output_decrement += 8 * sizeof(float);
        if (mc & (8 * sizeof(float))) {
            const float*restrict w = weights;
            const int32_t* dmap = widx_dmap;
            const uint32_t* nnzmap = nidx_nnzmap;
            size_t n = nc;
            do {
                uint32_t nnz = *nnzmap++;
                float32x4_t vacc0123 = vld1q_dup_f32(w); w += 1;
                float32x4_t vacc4567 = vacc0123;
                if XNN_LIKELY(nnz != 0) {
                    do {
                        const intptr_t diff = *dmap++;
                        const float32x4_t vi0123 = vld1q_f32(input);
                        const float32x4_t vi4567 = vld1q_f32(input + 4);
#ifdef PRESCALE_DIFF
                        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
#else
                        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff * (uintptr_t) m);
#endif
                        const float32x4_t vw = vld1q_dup_f32(w); w += 1;
                        vacc0123 = vmlaq_f32(vacc0123, vi0123, vw);
                        vacc4567 = vmlaq_f32(vacc4567, vi4567, vw);
                    } while (--nnz != 0);
                }
                //float32x4_t vout0123 = vminq_f32(vacc0123, vmax);
                //float32x4_t vout4567 = vminq_f32(vacc4567, vmax);
                //vout0123 = vmaxq_f32(vout0123, vmin);
                //vout4567 = vmaxq_f32(vout4567, vmin);
                vst1q_f32(output, vacc0123);
                vst1q_f32(output + 4, vacc4567);
                output = (float*restrict) ((uintptr_t) output + output_stride);
            } while (--n != 0);
            output = (float*restrict) ((uintptr_t) output - output_decrement);
            input += 8;
        }
        output_decrement += 4 * sizeof(float);
        if (mc & (4 * sizeof(float))) {
            const float*restrict w = weights;
            const int32_t* dmap = widx_dmap;
            const uint32_t* nnzmap = nidx_nnzmap;
            size_t n = nc;
            do {
                uint32_t nnz = *nnzmap++;
                float32x4_t vacc0123 = vld1q_dup_f32(w); w += 1;
                if XNN_LIKELY(nnz != 0) {
                    do {
                        const intptr_t diff = *dmap++;
                        const float32x4_t vi0123 = vld1q_f32(input);
#ifdef PRESCALE_DIFF
                        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
#else
                        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff * (uintptr_t) m);
#endif
                        const float32x4_t vw = vld1q_dup_f32(w); w += 1;
                        vacc0123 = vmlaq_f32(vacc0123, vi0123, vw);
                    } while (--nnz != 0);
                }
                //float32x4_t vout0123 = vminq_f32(vacc0123, vmax);
                //vout0123 = vmaxq_f32(vout0123, vmin);
                vst1q_f32(output, vacc0123);
                output = (float*restrict) ((uintptr_t) output + output_stride);
            } while (--n != 0);
            output = (float*restrict) ((uintptr_t) output - output_decrement);
            input += 4;
        }
        output_decrement += 2 * sizeof(float);
        if (mc & (2 * sizeof(float))) {
            const float*restrict w = weights;
            const int32_t* dmap = widx_dmap;
            const uint32_t* nnzmap = nidx_nnzmap;
            size_t n = nc;
            do {
                uint32_t nnz = *nnzmap++;
                float32x2_t vacc01 = vld1_dup_f32(w); w += 1;
                if XNN_LIKELY(nnz != 0) {
                    do {
                        const intptr_t diff = *dmap++;
                        const float32x2_t vi01 = vld1_f32(input);
#ifdef PRESCALE_DIFF
                        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
#else
                        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff * (uintptr_t) m);
#endif
                        const float32x2_t vw = vld1_dup_f32(w); w += 1;
                        vacc01 = vmla_f32(vacc01, vi01, vw);
                    } while (--nnz != 0);
                }
                //float32x2_t vout01 = vmin_f32(vacc01, vget_low_f32(vmax));
                //vout01 = vmax_f32(vout01, vget_low_f32(vmin));
                vst1_f32(output, vacc01);
                output = (float*restrict) ((uintptr_t) output + output_stride);
            } while (--n != 0);
            output = (float*restrict) ((uintptr_t) output - output_decrement);
            input += 2;
        }
        output_decrement += 1 * sizeof(float);
        if (mc & (1 * sizeof(float))) {
            const float*restrict w = weights;
            const int32_t* dmap = widx_dmap;
            const uint32_t* nnzmap = nidx_nnzmap;
            size_t n = nc;
            do {
                uint32_t nnz = *nnzmap++;
                float32x2_t vacc0 = vld1_dup_f32(w); w += 1;
                if XNN_LIKELY(nnz != 0) {
                    do {
                        const intptr_t diff = *dmap++;
                        const float32x2_t vi0 = vld1_dup_f32(input);
#ifdef PRESCALE_DIFF
                        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff);
#else
                        input = (const float*restrict) ((uintptr_t) input + (uintptr_t) diff * (uintptr_t) m);
#endif
                        const float32x2_t vw = vld1_dup_f32(w); w += 1;
                        vacc0 = vmla_f32(vacc0, vi0, vw);
                    } while (--nnz != 0);
                }
                //float32x2_t vout0 = vmin_f32(vacc0, vget_low_f32(vmax));
                //vout0 = vmax_f32(vout0, vget_low_f32(vmin));
                vst1_lane_f32(output, vacc0, 0);
                output = (float*restrict) ((uintptr_t) output + output_stride);
            } while (--n != 0);
            output = (float*restrict) ((uintptr_t) output - output_decrement);
            input += 1;
        }
    }
}