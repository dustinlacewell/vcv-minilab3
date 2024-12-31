import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';
import icon from "astro-icon";

// https://astro.build/config
export default defineConfig({
	site: 'https://minilab3.ldlework.com',
	integrations: [
		icon(),
		starlight({
			title: 'minilab3',
			social: {
				github: 'https://github.com/dustinlacewell/vcv-minilab3',
			},
			sidebar: [],
			customCss: [
				'./src/styles/custom.scss',
				'./src/styles/content.scss',
				'./src/styles/landing.scss',
			],
			components: {
				SocialIcons: './src/overrides/SocialLinks.astro',
				Header: './src/overrides/Header.astro',
			}
		}),
	],
});
