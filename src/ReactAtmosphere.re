[@bs.deriving jsConverter]
type layerState =
  | [@bs.as 0] Active
  | [@bs.as 1] TransitionExit;

type layerRenderArgsJs = {
  state: int,
  completeTransitionExit: unit => unit,
};
type layerRenderArgs = {
  state: layerState,
  completeTransitionExit: unit => unit,
};
type layerRenderJs = layerRenderArgsJs => React.element;
type layerRender = layerRenderArgs => React.element;
let layerRenderToLayerRenderJs =
    (render, {state, completeTransitionExit}: layerRenderArgsJs) => {
  render({
    state: Js.Option.getExn(layerStateFromJs(state)),
    completeTransitionExit,
  });
};

module Layer = {
  [@bs.module "react-atmosphere"] [@react.component]
  external make:
    (~render: layerRenderJs, ~transitionExit: bool=?, unit) => React.element =
    "Layer";

  let makeProps =
      (
        ~render: layerRender,
        ~transitionExit: option(bool)=?,
        ~key: option(string)=?,
        (),
      ) => {
    makeProps(
      ~render=layerRenderToLayerRenderJs(render),
      ~transitionExit?,
      ~key?,
      (),
    );
  };
};

module LayerContainer = {
  [@bs.module "react-atmosphere"] [@react.component]
  external make: unit => React.element = "LayerContainer";
};

module Popper = {
  module Options = {
    type t = {placement: option(string)};
  };

  module State = {
    type t;
  };
};

module PopperLayer = {
  type renderArgsJs = {
    state: int,
    completeTransitionExit: unit => unit,
    popperState: option(Popper.State.t),
  };
  type renderArgs = {
    state: layerState,
    completeTransitionExit: unit => unit,
    popperState: option(Popper.State.t),
  };

  [@bs.module "react-atmosphere"] [@react.component]
  external make:
    (
      ~id: string=?,
      ~reference: ReactDOMRe.Ref.currentDomRef,
      ~render: renderArgsJs => React.element,
      ~onOutsideClick: unit => unit=?,
      ~options: Popper.Options.t=?,
      ~transitionExit: bool=?,
      unit
    ) =>
    React.element =
    "PopperLayer";

  let makeProps =
      (
        ~id: option(string)=?,
        ~reference: ReactDOMRe.Ref.currentDomRef,
        ~render: renderArgs => React.element,
        ~onOutsideClick: option(unit => unit)=?,
        ~options: option(Popper.Options.t)=?,
        ~transitionExit: option(bool)=?,
        ~key: option(string)=?,
        (),
      ) => {
    makeProps(
      ~id?,
      ~reference,
      ~render=
        ({state, completeTransitionExit, popperState}) =>
          render({
            state: Js.Option.getExn(layerStateFromJs(state)),
            completeTransitionExit,
            popperState,
          }),
      ~onOutsideClick?,
      ~options?,
      ~transitionExit?,
      ~key?,
      (),
    );
  };
};

module Tooltip = {
  type renderTooltipProps = {popperState: option(Popper.State.t)};
  type configContext = {
    renderTooltip: (React.element, renderTooltipProps) => React.element,
    options: option(Popper.Options.t),
  };

  [@bs.module "react-atmosphere"] [@react.component]
  external configContext: React.Context.t(configContext) =
    "TooltipConfigContext";

  type tooltipRenderArgs('a, 'b, 'c, 'd) = {
    onMouseEnter: 'a => unit,
    onMouseLeave: 'b => unit,
    onFocus: 'c => unit,
    onBlur: 'd => unit,
    [@bs.as "aria-describedby"]
    ariaDescribedby: option(string),
    ref: ReactDOMRe.Ref.currentDomRef,
  };

  [@bs.module "react-atmosphere"] [@react.component]
  external make:
    (
      ~children: tooltipRenderArgs('a, 'b, 'c, 'd) => React.element,
      ~text: React.element,
      ~options: Popper.Options.t=?,
      unit
    ) =>
    React.element =
    "Tooltip";
};

module Dialog = {
  type renderBackdropArgsJs = {
    state: int,
    onClick: option(unit => unit),
  };
  type renderBackdropArgs = {
    state: layerState,
    onClick: option(unit => unit),
  };
  type configContext = {
    renderBackdrop: option(renderBackdropArgsJs => React.element),
    containerStyles: option(ReactDOMRe.Style.t),
    transitionDuration: option(int),
  };

  [@bs.module "react-atmosphere"] [@react.component]
  external configContext: React.Context.t(configContext) =
    "DialogConfigContext";

  type renderArgsJs = {state: int};
  type renderArgs = {state: layerState};

  module Layer = {
    [@bs.module "react-atmosphere"] [@react.component]
    external make:
      (
        ~render: renderArgsJs => React.element,
        ~state: int,
        ~completeTransitionExit: unit => unit,
        ~onBackdropClick: unit => unit=?,
        unit
      ) =>
      React.element =
      "DialogLayer";

    let makeProps =
        (
          ~render: renderArgs => React.element,
          ~state: layerState,
          ~completeTransitionExit: unit => unit,
          ~onBackdropClick: option(unit => unit)=?,
          ~key: option(string)=?,
          (),
        ) => {
      makeProps(
        ~render=
          ({state}) =>
            render({state: Js.Option.getExn(layerStateFromJs(state))}),
        ~state=layerStateToJs(state),
        ~completeTransitionExit,
        ~onBackdropClick?,
        ~key?,
        (),
      );
    };
  };

  [@bs.module "react-atmosphere"] [@react.component]
  external make:
    (
      ~render: renderArgsJs => React.element,
      ~onBackdropClick: unit => unit=?,
      unit
    ) =>
    React.element =
    "Dialog";

  let makeProps = (~render, ~onBackdropClick, ~key=?, ()) => {
    makeProps(
      ~render=
        ({state}) =>
          render({state: Js.Option.getExn(layerStateFromJs(state))}),
      ~onBackdropClick?,
      ~key?,
      (),
    );
  };
};

module API = {
  [@bs.module "react-atmosphere"] [@bs.scope "LayerAPI"]
  external pushLayer: (~render: layerRenderJs) => string = "pushLayer";
  [@bs.module "react-atmosphere"] [@bs.scope "LayerAPI"]
  external updateLayer: (~key: string, ~render: layerRenderJs) => unit =
    "updateLayer";
  [@bs.module "react-atmosphere"] [@bs.scope "LayerAPI"]
  external transitionExitLayer: (~key: string) => unit = "transitionExitLayer";
  [@bs.module "react-atmosphere"] [@bs.scope "LayerAPI"]
  external removeLayer: (~key: string) => unit = "removeLayer";

  let pushLayer = (~render: layerRender): string => {
    pushLayer(~render=layerRenderToLayerRenderJs(render));
  };
  let updateLayer = (~key: string, ~render: layerRender): unit => {
    updateLayer(~key, ~render=layerRenderToLayerRenderJs(render));
  };
};