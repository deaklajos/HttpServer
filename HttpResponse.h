#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QByteArray>

/**
 * @brief Class representing a HTTP 1.0 response.
 */
class HttpResponse
{
public:
    /**
     * @brief Creates a response from a request.
     *
     * If the request is too short(lacks the method or the resource badRequest() returned.<br>
     * If the requested resource could not be located notFound() returned.<br>
     * If the requested resource is the log file notFound() returned.<br>
     * If the request method is other than GET, HEAD, POST notImplemented() returned.<br>
     * If the requested resource exists but it cannot be opened internalServerError() returned.<br>
     * @param request Full request message acquired from the client.
     * @return Returns the response.<br>
     */
    static HttpResponse fromRequest(const QByteArray& request);

    // TODO make this private(only fromRequest calls this and it checks file existence)
    // or check for script existance
    /**
     * @brief Creates a response from a PHP script.
     *
     * This function uses the php executable to run the script.<br>
     * If the requested script fails on execution badRequest() returned.
     * @param scriptURI URI of to the script to be executed.
     * @param parameterString Parameters with the format of x-www-form-urlencoded.<br>
     * If this is an emty string than the request is identified as a GET otherwise POST request.
     * @return Returns the response.
     */
    static HttpResponse fromPHP(const QString& scriptURI, const QString& parameterString);

    // TODO these functions could be constexpr

    /**
     * @brief Creates a response with error code 400.
     * @param withBody If true a simple HTML response body provided.<br>
     * When responding to a HEAD request this should be false, otherwise
     * the client will refuse it.
     * @return Returns the response.
     */
    static HttpResponse badRequest(bool withBody);

    /**
     * @brief Creates a response with error code 404.
     * @param withBody If true a simple HTML response body provided.<br>
     * When responding to a HEAD request this should be false, otherwise
     * the client will refuse it.
     * @return Returns the response.
     */
    static HttpResponse notFound(bool withBody);

    /**
     * @brief Creates a response with error code 500.
     * @param withBody If true a simple HTML response body provided.<br>
     * When responding to a HEAD request this should be false, otherwise
     * the client will refuse it.
     * @return Returns the response.
     */
    static HttpResponse internalServerError(bool withBody);

    /**
     * @brief Creates a response with error code 501.
     * @param withBody If true a simple HTML response body provided.<br>
     * When responding to a HEAD request this should be false, otherwise
     * the client will refuse it.
     * @return Returns the response.
     */
    static HttpResponse notImplemented(bool withBody);

    /**
     * @brief Creates a response with error code 503.
     * @param withBody If true a simple HTML response body provided.<br>
     * When responding to a HEAD request this should be false, otherwise
     * the client will refuse it.
     * @return Returns the response.
     */
    static HttpResponse serviceUnavailabe(bool withBody);

    /**
     * @brief Getter for the respose data.
     * @return Returns the response data.
     */
    const QByteArray& getByteArray() const;

private:
    HttpResponse(const QByteArray& data);

    QByteArray data;
};

#endif
