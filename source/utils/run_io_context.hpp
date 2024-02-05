#ifndef TT_UTILS_RUN_IO_CONTEXT_HPP
#define TT_UTILS_RUN_IO_CONTEXT_HPP

namespace tt_program::utils
{

template<typename T>
class run_io_context_t
{
public:

    run_io_context_t(T & io_context)
        : m_io_context(io_context)
    {}

    void operator()() noexcept
    {
        while(true)
        {
            try
            {
                m_io_context.run(); // blocks here
                break;
            }
            catch(const std::exception & e)
            {
                // cout << e.what() << std::endl;
            }
            catch (...)
            {
                // cout << "Unknown error\n" << std::endl;
            }
        }
    }

private:

    T & m_io_context;
};

template<typename T>
run_io_context_t<T> run_io_context(T & io_context)
{
    return run_io_context_t<T>(io_context);
}

}

#endif // TT_UTILS_RUN_IO_CONTEXT_HPP