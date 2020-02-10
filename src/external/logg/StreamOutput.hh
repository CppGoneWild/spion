#ifndef LOGG_STREAMOUTPUT_HH__
#define LOGG_STREAMOUTPUT_HH__


#include "I_Output.hh"

#include <ostream>


namespace logg
{


class StreamOutput : public I_Output
{
public:
	StreamOutput()                     = default;
	StreamOutput(StreamOutput const &) = default;
	StreamOutput(StreamOutput &&)      = default;
	virtual ~StreamOutput()            = default;

	StreamOutput & operator=(StreamOutput const &) = default;
	StreamOutput & operator=(StreamOutput &&)      = default;

	StreamOutput(std::ostream &, bool color_mode);
	
protected:

	virtual void _log(Entry const &) override;

	std::ostream * _stream = nullptr;
	bool _color_mode = false;
};

extern StreamOutput cout;
extern StreamOutput cerr;
extern StreamOutput clog;


} // logg


#endif // LOGG_STREAMOUTPUT_HH__
