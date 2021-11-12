#include <asio.hpp>
#include <iostream>

class Ticker {
public:
    explicit Ticker(asio::io_service &ioService) : strand_{asio::make_strand(ioService)},
                                                   timer{ioService, asio::chrono::milliseconds{20}},
                                                   timerCopy{ioService, asio::chrono::milliseconds{20}} {
        tick(timer);
        tick(timerCopy);
    };

    void tick(asio::steady_timer &t) {
        std::cout << "class " << count_ << "\n";
        count_ += 20;
        if (count_ < 20'000) {
            t.expires_after(asio::chrono::milliseconds{20});
            auto cb = asio::bind_executor(strand_, [this, &t](const asio::error_code &ec) {
                if (!ec) {
                    tick(t);
                }
            });
            t.async_wait(std::move(cb));
        }
    }

private:
    asio::strand<asio::io_service::executor_type> strand_;
    asio::steady_timer timer;
    asio::steady_timer timerCopy;
    int count_{};
};

void print(const asio::error_code &ec, const std::shared_ptr<asio::steady_timer> &timer, int *counter) {
    std::cout << (*counter) << "\n";
    (*counter) += 290;

    if (*counter > 10'000)
        return;

    timer->expires_after(asio::chrono::milliseconds{290});
    timer->async_wait([=](const asio::error_code &ec) {
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
    asio::thread th{[&]() {
        ioService.run();
    }};
    ioService.run();
    th.join();
    return 0;
}