[@bs.deriving abstract]
type createOptions = {
  appId: string,
  [@bs.optional]
  oneGraphOrigin: string,
  [@bs.optional]
  oauthFinishOrigin: string,
  [@bs.optional]
  oauthFinishPath: string,
  [@bs.optional]
  saveAuthToStorage: bool,
  [@bs.optional]
  communicationMode: string,
  [@bs.optional]
  graphqlUrl: string,
};

type t;
[@bs.new] [@bs.module "onegraph-auth"]
external create: createOptions => t = "OneGraphAuth";

type token = {
  .
  "accessToken": string,
  "expireDate": int,
};

type authResponse = {. "token": token};

[@bs.get] external appId: t => string = "appId";

[@bs.send] external login: (t, string) => Js.Promise.t(unit) = "login";

[@bs.send]
external loginWithScopes: (t, string, array(string)) => Js.Promise.t(unit) =
  "login";

[@bs.send]
external logout_:
  (t, string, Js.nullable(string)) =>
  Js.Promise.t({. "response": {. "result": string}}) =
  "logout";

[@bs.send]
external isLoggedIn: (t, string) => Js.Promise.t(bool) = "isLoggedIn";

[@bs.send]
external allServices: (t, unit) => Js.Promise.t(array(string)) =
  "allServices";

type serviceStatus = {. "isLoggedIn": bool};

type servicesStatus = Js.Dict.t(serviceStatus);

[@bs.send]
external serviceStatus: (t, unit) => Js.Promise.t(array(servicesStatus)) =
  "serviceStatus";

[@bs.send] external setToken: (t, Js.Nullable.t(string)) => unit = "setToken";

[@bs.send] external authHeaders_: t => Js.Dict.t(string) = "authHeaders";

[@bs.send] external accessToken: t => Js.Nullable.t(token) = "accessToken";

[@bs.send]
external _findMissingAuthServices: (t, option('a)) => array(string) = "";

let findMissingAuthServices = (auth: t, resultIsh: option('a)): list(string) =>
  Array.to_list(_findMissingAuthServices(auth, resultIsh));

let logout = (auth, service, ~foreignUserId=?, ()) =>
  logout_(auth, service, Js.Nullable.fromOption(foreignUserId));

let authHeaders = auth => {
  let headers = authHeaders_(auth);
  Js.Dict.get(headers, "Authentication");
};

let clearToken = auth => {
  let appId = appId(auth);
  let storageKey = {j|oneGraph:$appId|j};
  Dom.Storage.removeItem(storageKey, Dom.Storage.localStorage);
  setToken(auth, Js.Nullable.fromOption(Some("{}")));
};
