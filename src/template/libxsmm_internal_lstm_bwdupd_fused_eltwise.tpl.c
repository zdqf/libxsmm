/******************************************************************************
** Copyright (c) 2016-2019, Intel Corporation                                **
** All rights reserved.                                                      **
**                                                                           **
** Redistribution and use in source and binary forms, with or without        **
** modification, are permitted provided that the following conditions        **
** are met:                                                                  **
** 1. Redistributions of source code must retain the above copyright         **
**    notice, this list of conditions and the following disclaimer.          **
** 2. Redistributions in binary form must reproduce the above copyright      **
**    notice, this list of conditions and the following disclaimer in the    **
**    documentation and/or other materials provided with the distribution.   **
** 3. Neither the name of the copyright holder nor the names of its          **
**    contributors may be used to endorse or promote products derived        **
**    from this software without specific prior written permission.          **
**                                                                           **
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       **
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         **
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     **
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      **
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    **
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED  **
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR    **
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
******************************************************************************/
/* Evangelos Georganas (Intel Corp.), Alexander Heinecke (Intel Corp.)
******************************************************************************/

{
  libxsmm_blasint _k, _j;
  __m512 _vdout, _vdh, _vo, _vt1, _vt2, _vco, _vdcs, _vdcp, _vi, _vci, _vdci, _vdi, _vcps, _vf, _vdf, _vdp;
  element_input_type* _dout = &LIBXSMM_VLA_ACCESS(2, dout, in, ik, K);
  element_input_type* _dh = &LIBXSMM_VLA_ACCESS(3, dh, j, in, ik, N, K);
  element_input_type* _o = &LIBXSMM_VLA_ACCESS(3, o, j, in, ik, N, K);
  element_input_type* _co = &LIBXSMM_VLA_ACCESS(3, co, j, in, ik, N, K);
  element_input_type* _dcs = &LIBXSMM_VLA_ACCESS(2, dcs, in, ik, K);
  element_input_type* _i = &LIBXSMM_VLA_ACCESS(3, i, j, in, ik, N, K);
  element_input_type* _ci = &LIBXSMM_VLA_ACCESS(3, ci, j, in, ik, N, K);
  element_input_type* _dci = &LIBXSMM_VLA_ACCESS(2, dci, in, ik, K);
  element_input_type* _di = &LIBXSMM_VLA_ACCESS(2, di, in, ik, K);
  element_input_type* _cps = cps_ptr;
  element_input_type* _f = &LIBXSMM_VLA_ACCESS(3, f, j, in, ik, N, K);
  element_input_type* _df = &LIBXSMM_VLA_ACCESS(2, df, in, ik, K);
  element_input_type* _dp = &LIBXSMM_VLA_ACCESS(2, dp, in, ik, K);
  element_input_type* _dcp = &LIBXSMM_VLA_ACCESS(2, dcp, in, ik, K);
  const __m512 _vneg_ones = _mm512_set1_ps( (float)-1.0 );
  const __m512 _vones = _mm512_set1_ps( (float)1.0 );
  if (j == t-1) {
    for ( _j = 0; _j < bn; ++_j ) {
      LIBXSMM_PRAGMA_UNROLL_N(4)
      for ( _k = 0; _k < bk; _k += 16 ) {
        _vdout = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_dh[(_j*K)+_k] );
        _vo = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_o[(_j*K)+_k] );
        _vt1 = _mm512_mul_ps( _vdout, _vo  );
        _vco = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_co[(_j*K)+_k] );
        _vt2 = _mm512_fnmsub_ps ( _vco, _vco, _vneg_ones);
        _vt1 = _mm512_mul_ps( _vt1, _vt2 );
        _vdcs = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_dcs[(_j*K)+_k] );
        _vdcp = _mm512_add_ps( _vdcs, _vt1 );
        _vi = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_i[(_j*K)+_k] );
        _vt1 = _mm512_mul_ps( _vi, _vdcp );
        _vci = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_ci[(_j*K)+_k] );
        _vt2 = _mm512_fnmsub_ps ( _vci, _vci, _vneg_ones);
        _vdci = _mm512_mul_ps( _vt1, _vt2 );
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_dci[(_j*K)+_k], _vdci );
        _vt1 = _mm512_mul_ps( _vci, _vdcp );
        _vt2 = _mm512_sub_ps( _vones, _vi );
        _vdi = _mm512_mul_ps( _vi, _vt2);
        _vdi = _mm512_mul_ps( _vdi, _vt1);
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_di[(_j*K)+_k], _vdi );
        _vcps = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_cps[(_j*K)+_k] );
        _vt1 = _mm512_mul_ps( _vcps, _vdcp );
        _vf = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_f[(_j*K)+_k] );
        _vt2 = _mm512_sub_ps( _vones, _vf );
        _vdf = _mm512_mul_ps( _vf, _vt2);
        _vdf = _mm512_mul_ps( _vdf, _vt1);
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_df[(_j*K)+_k], _vdf );
        _vt1 = _mm512_mul_ps( _vdout, _vco);
        _vt2 = _mm512_sub_ps( _vones, _vo );
        _vt2 = _mm512_mul_ps( _vo, _vt2);
        _vdp = _mm512_mul_ps( _vt1, _vt2 );
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_dp[(_j*K)+_k], _vdp );
        _vdcp = _mm512_mul_ps( _vdcp, _vf);
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_dcp[(_j*K)+_k], _vdcp );
      }
    }
  } else {
    for ( _j = 0; _j < bn; ++_j ) {
       LIBXSMM_PRAGMA_UNROLL_N(4)
       for ( _k = 0; _k < bk; _k += 16 ) {
        _vdout = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_dout[(_j*K)+_k] );
        _vdh = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_dh[(_j*K)+_k] );
        _vdout = _mm512_add_ps( _vdout, _vdh );
        _vo = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_o[(_j*K)+_k] );
        _vt1 = _mm512_mul_ps( _vdout, _vo  );
        _vco = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_co[(_j*K)+_k] );
        _vt2 = _mm512_fnmsub_ps ( _vco, _vco, _vneg_ones);
        _vt1 = _mm512_mul_ps( _vt1, _vt2 );
        _vdcp = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_dcp[(_j*K)+_k] );
        _vdcp = _mm512_add_ps( _vdcp, _vt1 );
        _vi = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_i[(_j*K)+_k] );
        _vt1 = _mm512_mul_ps( _vi, _vdcp );
        _vci = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_ci[(_j*K)+_k] );
        _vt2 = _mm512_fnmsub_ps ( _vci, _vci, _vneg_ones);
        _vdci = _mm512_mul_ps( _vt1, _vt2 );
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_dci[(_j*K)+_k], _vdci );
        _vt1 = _mm512_mul_ps( _vci, _vdcp );
        _vt2 = _mm512_sub_ps( _vones, _vi );
        _vdi = _mm512_mul_ps( _vi, _vt2);
        _vdi = _mm512_mul_ps( _vdi, _vt1);
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_di[(_j*K)+_k], _vdi );
        _vcps = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_cps[(_j*K)+_k] );
        _vt1 = _mm512_mul_ps( _vcps, _vdcp );
        _vf = LIBXSMM_INTRINSICS_MM512_LOAD_PS( &_f[(_j*K)+_k] );
        _vt2 = _mm512_sub_ps( _vones, _vf );
        _vdf = _mm512_mul_ps( _vf, _vt2);
        _vdf = _mm512_mul_ps( _vdf, _vt1);
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_df[(_j*K)+_k], _vdf );
        _vt1 = _mm512_mul_ps( _vdout, _vco);
        _vt2 = _mm512_sub_ps( _vones, _vo );
        _vt2 = _mm512_mul_ps( _vo, _vt2);
        _vdp = _mm512_mul_ps( _vt1, _vt2 );
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_dp[(_j*K)+_k], _vdp );
        _vdcp = _mm512_mul_ps( _vdcp, _vf);
        LIBXSMM_INTRINSICS_MM512_STREAM_PS( &_dcp[(_j*K)+_k], _vdcp );
      }
    }
  }
}
