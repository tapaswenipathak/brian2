{% macro main() %}

    // USES_VARIABLES { _t, _i, t, _spikes, _count,
    //                  _source_start, _source_end}

    if (_num_spikes > 0)
    {
        // For subgroups, we do not want to record all spikes
        // We assume that spikes are ordered
        // TODO: Will this assumption ever be violated?
        int _start_idx = 0;
        int _end_idx = - 1;
        for(int _idx=0; _idx<_num_spikes; _idx++)
        {
            const int _element_idx = _spikes[_idx];
            if (_element_idx >= _source_start) {
                _start_idx = _idx;
                break;
            }
        }
        for(int _idx=_start_idx; _idx<_num_spikes; _idx++)
        {
            const int _element_idx = _spikes[_idx];
            if (_element_idx >= _source_end) {
                _end_idx = _idx;
                break;
            }
        }
        if (_end_idx == -1)
            _end_idx =_num_spikes;
        _num_spikes = _end_idx - _start_idx;
        if (_num_spikes > 0) {
            // Get the current length and new length of t and i arrays
            const int _curlen = _t.attr("shape")[0];
            const int _newlen = _curlen + _num_spikes;
            // Resize the arrays
            py::tuple _newlen_tuple(1);
            _newlen_tuple[0] = _newlen;
            _t.mcall("resize", _newlen_tuple);
            _i.mcall("resize", _newlen_tuple);
            // Get the potentially newly created underlying data arrays
            double *_t_data = (double*)(((PyArrayObject*)(PyObject*)_t.attr("data"))->data);
            // TODO: How to get the correct datatype automatically here?
            npy_int64 *_i_data = (npy_int64*)(((PyArrayObject*)(PyObject*)_i.attr("data"))->data);
            // Copy the values across
            for(int _idx=_start_idx; _idx<_end_idx; _idx++)
            {
                const int _element_idx = _spikes[_idx];
                _t_data[_curlen + _idx - _start_idx] = t;
                _i_data[_curlen + _idx - _start_idx] = _element_idx - _source_start;
                _count[_element_idx - _source_start]++;
            }
        }
	}
{% endmacro %}

{% macro support_code() %}
{% endmacro %}
