[@bs.deriving jsConverter]
type layerState =
  | [@bs.as 0] Active
  | [@bs.as 1] TransitionExit;

type layerRenderArgs = {
  state: layerState,
  completeTransitionExit: unit => unit,
};
type layerRender = layerRenderArgs => React.element;

type layer = {
  key: string,
  state: layerState,
  render: layerRender,
};

module Layer = {
  [@bs.module "react-atmosphere"] [@react.component]
  external make:
    (~render: layerRender, ~transitionExit: bool=?, unit) => React.element =
    "Layer";
};

module LayerContainer = {
  [@bs.module "react-atmosphere"] [@react.component]
  external make: unit => React.element = "LayerContainer";
};

module Popper = {
  module Options = {
    [@bs.deriving jsConverter]
    type placement = [
      | [@bs.as "top"] `Top
      | [@bs.as "bottom"] `Bottom
      | [@bs.as "left"] `Left
      | [@bs.as "right"] `Right
    ];

    type t = {placement: option(placement)};
  };

  module State = {
    type t;
  };
};

module PopperLayer = {
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
      ~render: renderArgs => React.element,
      ~onOutsideClick: unit => unit=?,
      ~options: Popper.Options.t=?,
      ~transitionExit: bool=?,
      unit
    ) =>
    React.element =
    "PopperLayer";
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
  type renderBackdropArgs = {
    state: layerState,
    onClick: option(unit => unit),
  };
  type configContext = {
    renderBackdrop: option(renderBackdropArgs => React.element),
    containerStyles: option(ReactDOMRe.Style.t),
    transitionDuration: option(int),
  };

  [@bs.module "react-atmosphere"] [@react.component]
  external configContext: React.Context.t(configContext) =
    "DialogConfigContext";

  type renderArgs = {state: layerState};

  module Layer = {
    [@bs.module "react-atmosphere"] [@react.component]
    external make:
      (
        ~render: renderArgs => React.element,
        ~state: layerState,
        ~completeTransitionExit: unit => unit,
        ~onBackdropClick: unit => unit=?,
        unit
      ) =>
      React.element =
      "DialogLayer";
  };

  [@bs.module "react-atmosphere"] [@react.component]
  external make:
    (
      ~render: renderArgs => React.element,
      ~onBackdropClick: unit => unit=?,
      unit
    ) =>
    React.element =
    "Dialog";
};

module API = {
  [@bs.module "react-atmosphere"] [@bs.scope "LayerAPI"]
  external pushLayer: (~render: layerRender) => string = "pushLayer";
  [@bs.module "react-atmosphere"] [@bs.scope "LayerAPI"]
  external updateLayer: (~key: string, ~render: layerRender) => unit =
    "updateLayer";
  [@bs.module "react-atmosphere"] [@bs.scope "LayerAPI"]
  external transitionExitLayer: (~key: string) => unit = "transitionExitLayer";
  [@bs.module "react-atmosphere"] [@bs.scope "LayerAPI"]
  external removeLayer: (~key: string) => unit = "removeLayer";
};