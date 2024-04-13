void DevToolsHttpHandler::RespondToJsonList(
    int connection_id,
    const std::string& host,
    DevToolsAgentHost::List hosts) {
  DevToolsAgentHost::List agent_hosts = std::move(hosts);
  std::sort(agent_hosts.begin(), agent_hosts.end(), TimeComparator);
  base::ListValue list_value;
  for (auto& agent_host : agent_hosts)
    list_value.Append(SerializeDescriptor(agent_host, host));
  SendJson(connection_id, net::HTTP_OK, &list_value, std::string());
}
