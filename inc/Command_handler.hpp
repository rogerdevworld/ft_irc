











#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

# include "Client.hpp"
# include "Server.hpp"
# include "command_excepts.hpp"
# include <exception>
# include <string>
# include <vector>

typedef void	(*HandlerExecutor)(Server &, Client &,
		const std::vector<std::string> &);

struct			Command
{
	std::string name;
	std::vector<std::string> params;
};

class AbstractCommandHandler
{
  private:
	AbstractCommandHandler(const AbstractCommandHandler &);

	AbstractCommandHandler &operator=(const AbstractCommandHandler &);

  protected:
	Server &_server;
	Client &_client;

  public:
	AbstractCommandHandler(Server &server, Client &client);

	virtual ~AbstractCommandHandler();

	void setCommandName(const std::string commandName);

	const std::string &getCommandName() const;

	static Command parseLine(const std::string &line);

	static void executeCommand(Server &server, Client &client,
		const std::string commandName, const std::vector<std::string> &params);

	virtual void execute(const std::vector<std::string> &params) = 0;
};



class PassCommandHandler : public AbstractCommandHandler
{
  public:
	PassCommandHandler(Server &server, Client &client);

	~PassCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class UserCommandHandler : public AbstractCommandHandler
{
  public:
	UserCommandHandler(Server &server, Client &client);

	~UserCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class NickCommandHandler : public AbstractCommandHandler
{
  public:
	NickCommandHandler(Server &server, Client &client);

	~NickCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class CapCommandHandler : public AbstractCommandHandler
{
  public:
	CapCommandHandler(Server &server, Client &client);

	~CapCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class PingCommandHandler : public AbstractCommandHandler
{
  public:
	PingCommandHandler(Server &server, Client &client);

	~PingCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class QuitCommandHandler : public AbstractCommandHandler
{
  public:
	QuitCommandHandler(Server &server, Client &client);

	~QuitCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class AChannelCommandHandler : public AbstractCommandHandler
{
  protected:
	std::vector<std::string> parseChannelList(const std::string &channelList);

  public:
	AChannelCommandHandler(Server &server, Client &client);
	virtual ~AChannelCommandHandler();

	virtual void execute(const std::vector<std::string> &params) = 0;
};



 class ListCommandHandler : public AChannelCommandHandler
 {
   public:
	ListCommandHandler(Server &server, Client &client);

	~ListCommandHandler();

	void execute(const std::vector<std::string> &params);
 };

 

 class WhoChannelCommandHandler : public AChannelCommandHandler	
 {
   public:
	WhoChannelCommandHandler(Server &server, Client &client);

	~WhoChannelCommandHandler();

	void execute(const std::vector<std::string> &params);
 };



class JoinChannelCommandHandler : public AChannelCommandHandler
{
  public:
	JoinChannelCommandHandler(Server &server, Client &client);

	~JoinChannelCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class PartChannelCommandHandler : public AChannelCommandHandler
{
  public:
	PartChannelCommandHandler(Server &server, Client &client);

	~PartChannelCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class KickChannelCommandHandler : public AChannelCommandHandler
{
  public:
	KickChannelCommandHandler(Server &server, Client &client);

	~KickChannelCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class TopicCommandHandler : public AChannelCommandHandler
{
  public:
	TopicCommandHandler(Server &server, Client &client);

	~TopicCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class ModeCommandHandler : public AChannelCommandHandler
{
  public:
	ModeCommandHandler(Server &server, Client &client);

	~ModeCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class InviteCommandHandler : public AChannelCommandHandler
{
  public:
	InviteCommandHandler(Server &server, Client &client);

	~InviteCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class PrivmsgCommandHandler : public AChannelCommandHandler
{
  public:
	PrivmsgCommandHandler(Server &server, Client &client);

	~PrivmsgCommandHandler();

	void execute(const std::vector<std::string> &params);
};



class WhoisCommandHandler : public AbstractCommandHandler
{
  public:
	WhoisCommandHandler(Server &server, Client &client);

	~WhoisCommandHandler();

	void execute(const std::vector<std::string> &params);
};

#endif