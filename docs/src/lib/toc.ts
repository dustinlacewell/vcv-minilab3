import { type MarkdownHeading } from "astro";

export interface TocItem extends MarkdownHeading {
    children: TocItem[];
}

interface TocOpts {
    minHeadingLevel: number;
    maxHeadingLevel: number;
    title: string;
}

export function injectChild(items: TocItem[], item: TocItem): void {
    const lastItem = items.at(-1);
    if (!lastItem || lastItem.depth >= item.depth) {
        items.push(item);
    } else {
        return injectChild(lastItem.children, item);
    }
}

export function generateToC(
    headings: MarkdownHeading[],
    { minHeadingLevel, maxHeadingLevel, title }: TocOpts,
) {
    headings = headings.filter(
        ({ depth }) => depth >= minHeadingLevel && depth <= maxHeadingLevel,
    );
    const toc: Array<TocItem> = [
        { depth: 2, slug: "_top", text: title, children: [] },
    ];
    for (const heading of headings) {
        injectChild(toc, { ...heading, children: [] });
    }
    return toc;
}

export function getToC({ entry, lang, headings }: any) {
    const tocConfig = {
        maxHeadingLevel: 3,
        minHeadingLevel: 1,
        title: "Overview",
    };

    return {
        ...tocConfig,
        items: generateToC(headings, {
            ...tocConfig,
            title: "",
        }),
    };
}