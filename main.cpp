#include <asio.hpp>
#include <iostream>

class Ticker {
public:
    explicit Ticker(asio::io_service &ioService) : timer{ioService, asio::chrono::milliseconds {200}} {
        tick();
    };
    void tick()
    {
        std::cout << "class " << count_ << "\n";
        count_ += 200;
        if (count_ < 10'000) {
            timer.expires_after(asio::chrono::milliseconds{200});
            timer.async_wait([this](const asio::error_code& ec) {
                if (!ec) {
                    tick();
                }
            });
        }
    }

private:
    asio::steady_timer timer;
    int count_{};
};

void print(const asio::error_code& ec, const std::shared_ptr<asio::steady_timer>& timer, int* counter)
{
    std::cout << (*counter) << "\n";
    (*counter) += 290;

    if (*counter > 10'000)
        return;

    timer->expires_after(asio::chrono::milliseconds {290});
    timer->async_wait([=](const asio::error_code& ec) {
        print(ec, timer, counter);
    });
}

int main() {
    asio::io_service ioService{};

    auto t{std::make_shared<asio::steady_timer>(ioService)};

    int c{};
    print(asio::error_code{}, t, &c);
    new Ticker{ioService};
    std::cout << "end\n";
    ioService.run();
    return 0;
}